#include "helpers.h"

using namespace std;

void usage(char *file) {
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[]) {
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	int sockfd, ret;
	struct sockaddr_in serv_addr;
	fd_set read_fds, tmp_fds;
	char buffer[BUFLEN];

	if (argc < 4) {
		usage(argv[0]);
	}

	// se goleste multimea de descriptori de citire (read_fds) si multimea temporarara (tmp_fds)
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket error");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	ret = inet_aton(argv[2], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton error");
	
	ret = connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect error");

	// dezactivez algoritmul lui Nagle
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)1, sizeof(int));

	char *ID = argv[1];
	struct send_request send_req;
	memcpy(send_req.ID, ID, 10);
	send_req.type = 0;
	strcpy(send_req.topic, "\0");
	send_req.flag = 0;
	ret = send(sockfd, &send_req, sizeof(send_req), 0);
	DIE(ret < 0, "send error");

	FD_SET(sockfd, &read_fds);
	FD_SET(0, &read_fds);

	while (1) {
		tmp_fds = read_fds; 
		ret = select(sockfd + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select error");
		
		// preluare mesaj de la stdin
		if (FD_ISSET(0, &tmp_fds)) {
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);
			// DISCONNECT
			if (strncmp(buffer, "exit", 4) == 0) {
				struct send_request send_req;
				memcpy(send_req.ID, ID, 10);
				send_req.type = 3;
				strcpy(send_req.topic, "\0");
				send_req.flag = 0;

				ret = send(sockfd, &send_req, sizeof(send_req), 0);
				DIE(ret < 0, "send error");
				break;
			}
			// SUBSCRIBE
			else if (strncmp(buffer, "subscribe", 9) == 0) {
				struct send_request send_req;
				send_req.flag = buffer[strlen(buffer) - 2] == '1' ? 1 : 0;
				memcpy(send_req.ID, ID, 10);
				send_req.type = 1;
				string topic = string(buffer + 10);
				topic = topic.substr(0, topic.find(' '));
				topic.append(1, '\0');
				strcpy(send_req.topic, topic.c_str());
				
				ret = send(sockfd, &send_req, sizeof(send_request), 0);
				DIE(ret < 0, "send error");

				printf("Subscribed to topic.\n");
			}
			// UNSUBSCRIBE
			else if (strncmp(buffer, "unsubscribe", 11) == 0) {
				// send unsubscribe request
				struct send_request send_req;
				memcpy(send_req.ID, ID, 10);
				send_req.type = 2;
				string topic = string(buffer + 12);
				topic = topic.substr(0, topic.length() - 1);
				topic.append(1, '\0');
				strcpy(send_req.topic, topic.c_str());
				send_req.flag = 0;

				ret = send(sockfd, &send_req, sizeof(send_request), 0);
				DIE(ret < 0, "send error");
			
				printf("Unsubscribed to topic.\n");
			}
		}

		// preluare mesaj de la server (string/DISCONNECT)
		if (FD_ISSET(sockfd, &tmp_fds)) {
			memset(buffer, 0, BUFLEN);
			ret = recv(sockfd, buffer, sizeof(buffer), 0);
			DIE(ret < 0, "read error");
			if (strstr(buffer, "DISCONNECT") != nullptr) {
				break;
			}
			printf("%s", buffer);
		}
	}

	// inchid socketul
	close(sockfd);
	return 0;
}
