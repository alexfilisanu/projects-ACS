#include "helpers.h"

using namespace std;

struct Client {
	bool online;
	int socket;
	list<char *> posts;
};

struct Topic {
	char ID[10];
	int SF;
};

void usage(char *file) {
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}

// functie ce returneaza informatiile din payload in format human-readable 
char *getPost(struct sockaddr_in cli_addr, struct UDPmessage *msg) {
	char *post = (char *)malloc(BUFLEN + 100);
	memset(post, 0, BUFLEN + 100);
	switch (msg->type)	{
	case 0:	{	// INT
		int n = htonl(*(int *)(msg->data + 1));
		if (msg->data[0]) {
			n *= -1;
		}
		sprintf(post, "%s:%d - %s - %s - %d\n",
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),
			msg->topic, "INT", n);	
		}	
		break;
	
	case 1: { 	// SHORT_REAL
		double n = (double)htons(*(int *)(msg->data)) / 100;
		sprintf(post, "%s:%d - %s - %s - %.2f\n",
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),
			msg->topic, "SHORT_REAL", n);
		}
		break;

	case 2: {		// FLOAT
		double n = (double)htonl(*(int *)(msg->data + 1));
		if (msg->data[0]) {
			n *= -1;
		}
		for (int i = 0; i < msg->data[5]; i++) {
			n /= 10;
		}
		sprintf(post, "%s:%d - %s - %s - %f\n",
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),
			msg->topic, "FLOAT", n);
		}
		break;

	case 3: {		// STRING
		sprintf(post, "%s:%d - %s - %s - %s\n",
			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),
			msg->topic, "STRING", msg->data);
		}
		break;

	default:
		DIE(1, "type error");
		break;
	}
	
	return post;
}

int main(int argc, char *argv[]) {
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	int sockUDP, sockTCP, ret, newsock;
	char buffer[BUFLEN];
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	fd_set read_fds; 	// multimea de citire folosita in select()
	fd_set tmp_fds; 	// multimea folosita temporar
	int fdmax; 			// valoare maxima fd din multimea read_fds

	if (argc < 2) {
		usage(argv[0]);
	}

	// se goleste multimea de descriptori de citire (read_fds) si multimea temporarara (tmp_fds)
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	sockUDP = socket(AF_INET, SOCK_DGRAM, 0);
	DIE(sockUDP < 0, "open UDP socket error");
	
	sockTCP = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockTCP < 0, "open TCP socket error");

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockUDP, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind UDP socket error");
	
	ret = bind(sockTCP, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(ret < 0, "bind TCP socket error");
	
	ret = listen(sockTCP, MAX_CLIENTS);
	DIE(ret < 0, "listen TCP socket error");
	
	// se adauga noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
	FD_SET(sockUDP, &read_fds);
	FD_SET(sockTCP, &read_fds);
	FD_SET(0, &read_fds);
	fdmax = sockUDP > sockTCP ? sockUDP : sockTCP;

	// initializez cele 2 map-uri pe care o sa le folosesc pentru gestionarea mesajelor 
	map<string, Client> clients;
	map<string, list<Topic>> topics;

	// main loop
	while (1) {
		tmp_fds = read_fds; 
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select error");

		// in cazul in care primesc comanda exit, deconectez toti clientii si ies din while
		if (FD_ISSET(0, &tmp_fds)) {
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);
			if (strncmp(buffer, "exit", 4) == 0) {
				for (auto c : clients) {
					if (c.second.online) {
						send(c.second.socket, "DISCONNECT", 11, 0);
					}
				}
				break;
			}
		}

		for (int i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if (i == sockTCP) {
					// a venit o cerere de conexiune pe socketul inactiv (cel cu listen),
					// pe care serverul o accepta
					clilen = sizeof(cli_addr);
					newsock = accept(sockTCP, (struct sockaddr*) &cli_addr, &clilen);
					DIE(newsock < 0, "accept error");

					// se adauga noul socket intors de accept() la multimea descriptorilor de citire
					FD_SET(newsock, &read_fds);
					if (newsock > fdmax) {
						fdmax = newsock;
					}
				} else if (i == sockUDP) {
					// s-au primit mesaje de tip UDP
					ret = recvfrom(sockUDP, buffer, BUFLEN, 0, (struct sockaddr *) &cli_addr, &clilen);
					DIE(ret < 0, "read UDP socket error");
					buffer[ret] = '\0';

					// preiaul mesajul de tip UDP din buffer
					struct UDPmessage *msg = (struct UDPmessage *)buffer;
					char *topic = msg->topic;
					char *post = getPost(cli_addr, msg);

					if (topics.find(topic) != topics.end()) {
						auto topicList = topics[topic];
						for (auto t : topicList) {
							char ID[10];
							strcpy(ID, t.ID);
							// daca clientul este online -> forward post
							if (clients[ID].online) {
								send(clients[ID].socket, post, BUFLEN, 0);
							}
							// altfel, daca SF = 1 -> store post
							else if (t.SF) {
								clients[ID].posts.push_back(post);
							}
						}
					}
				} else {
					// s-au primit date pe unul din socketii de client,
					// asa ca serverul trebuie sa le receptioneze
					memset(buffer, 0, BUFLEN);
					ret = recv(i, buffer, BUFLEN, 0);
					DIE(ret < 0, "recv error");
					
					struct send_request *request = (struct send_request *)buffer;
					char ID[10];
					strcpy(ID, request->ID);
					
					// verific tipul requestului
					switch (request->type) {
					case 0:		// IDENTIFY
						// daca clientul e online -> deja exista clientul -> afisez mesajul ca e deja conectat
						// + send DISCONNECT
						if (clients[ID].online) {
							printf("Client %s already connected.\n", ID);
							send(i, "DISCONNECT", 11, 0);
							FD_CLR(i, &read_fds); 
							FD_CLR(i, &tmp_fds);
							close(i);
						} else {
							// daca clientul exista si nu era online / nu exista, ii actualizez campurile 
							// online si socket
							clients[ID].online = true;
							clients[ID].socket = i;
							// afisez mesajul ca noul client s-a conectat
							getpeername(i, (struct sockaddr *) &cli_addr, &clilen);
							printf("New client %s connected from %s:%d.\n", ID, 
								inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
							// trimit toate post urile din lista (acest eveniment se intampla numai in cazul
							// in care clientul exista, in caz contrar lista e vida si nu se intampla nimic)
							for (auto post : clients[ID].posts) {
								send(i, post, BUFLEN, 0); 
							}
						}
						break;
					
					case 1:		// SUBSCRIBE
						// adug topicul listei corespunzatoare din topics
						Topic topic;
						strcpy(topic.ID, ID);
						topic.SF = request->flag;
						topics[request->topic].push_back(topic);
						break;

					case 2: { 	// UNSUBSCRIBE
							// sterg topicul din lista corespunzatoare
							auto topicList = topics[request->topic];
							for (auto t = topicList.begin(); t != topicList.end(); t++) {
								if (strcmp((*t).ID, ID) == 0) {
									topicList.erase(t);
									break;
								}
							}
						}
						break;

					case 3: 	// DISCONNECT
						// setez campurile online si socket ale clientului + afisez deconectarea
						clients[ID].online = false;
						clients[ID].socket = 0;
						FD_CLR(i, &read_fds); 
						FD_CLR(i, &tmp_fds);
						close(i);
						printf("Client %s disconnected.\n", ID);
						break;

					default:
						DIE(1, "type error");
						break;
					}					
				}
			}
		}
	}

	// inchid socketurile
	close(sockUDP);
	close(sockTCP);
	return 0;
}

