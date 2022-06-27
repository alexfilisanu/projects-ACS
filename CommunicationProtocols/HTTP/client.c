#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdbool.h>
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

#include "helpers.h"
#include "parson.h"
#include "requests.h"

// functia corepunzatoare comenzii register
void registerFunction() {
	int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		exit(1);
	}
	char *username = calloc(20, sizeof(char));
	if (!username) {
		exit(1);
	}
	char *password = calloc(20, sizeof(char));
	if (!password) {
		exit(1);
	}
	printf("username=");
	scanf("%s", username);
	printf("password=");
	scanf("%s", password);

	JSON_Value *value = json_value_init_object();
	if (!value) {
		exit(1);
	}
	JSON_Object *object = json_value_get_object(value);
	if (!object) {
		exit(1);
	}
	json_object_set_string(object, "username", username);
	json_object_set_string(object, "password", password);
	char *form_data = json_serialize_to_string(value);
	char *message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/register",
		"application/json", &form_data, 1, NULL, 0, NULL);
	send_to_server(sockfd, message);
	char *response = receive_from_server(sockfd);
	// daca in response exista "is taken!", inseamna ca username-ul deja exista
	// formatul erorii fiind {"error":"The username %s is taken!"}
	if (strstr(response, "is taken!")) {
		printf("Username-ul este deja folosit de catre cineva!\n");
	} else {
		printf("Inregistrare realizata cu succes!\n");
	}

	free(username);
	free(password);
	json_value_free(value);
	close_connection(sockfd);
}

// functia corepunzatoare comenzii login
void loginFunction(int *loginCheck, int *libraryCheck, char **cookies) {
	// daca niciun utilizator este deja logat, incerc sa-l conectez, altfel nu
	if (*loginCheck) {
		printf("Un alt utilizator este deja logat!\n");
	} else {	 
		int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			exit(1);
		}
		char *username = calloc(20, sizeof(char));
		if (!username) {
			exit(1);
		}
		char *password = calloc(20, sizeof(char));
		if (!password) {
			exit(1);
		}
		printf("username=");
	    scanf("%s", username);
		printf("password=");
		scanf("%s", password);

		JSON_Value *value = json_value_init_object();
		if (!value) {
			exit(1);
		}
		JSON_Object *object = json_value_get_object(value);
		if (!object) {
			exit(1);
		}
		json_object_set_string(object, "username", username);
		json_object_set_string(object, "password", password);
		char *form_data = json_serialize_to_string(value);

		char *message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/login", 
			"application/json", &form_data, 1, NULL, 0, NULL);
		send_to_server(sockfd, message);
		char *response = receive_from_server(sockfd);
					
		// verific daca datele de logare sunt bune, caz in care setez
		// loginCheck la 1 si libraryCheck la 0
		char *cookie = strstr(response, "Set-Cookie: ");
		if (!cookie) {
			printf("Credentialele nu se potrivesc!\n");
		} else {
			// cookie este de forma Set-Cookie: cookie;
			strtok(cookie, ";");
			strcpy(cookies[0], cookie + 12);
			if (cookies[0]) {
				printf("Logare realizata cu succes!\n");
				*loginCheck = 1;
				*libraryCheck = 0;
			}
		}
		free(username);
		free(password);
		json_value_free(value);
		close_connection(sockfd);
	} 
}

// functia corepunzatoare comenzii enter_library
void enter_libraryFunction(int *loginCheck, int *libraryCheck, char **cookies, char *jwt) {
	// aceasta comanda se realiza doar daca logarea a fost efectuata
	if (*loginCheck) {
		int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			exit(1);
		}
		char *message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access",
			NULL, cookies, 1, jwt);
		send_to_server(sockfd, message);
		char *response = receive_from_server(sockfd);
		// verific daca am acces la library
		char *token = strstr(response, "token");
		if (!token) {
			printf("Nu este permis accesul la biblioteca!\n");
		} else {
			// daca am setez valoarea libraryCheck la 1
			*libraryCheck = 1;
			// token este de forma: token":"jwt"}
			memset(jwt, 0, BUFLEN);
			strcpy(jwt, token + 8);
			jwt[strlen(jwt) - 2] = '\0';
			printf("Biblioteca accesata cu succes!\n");
		}
		close_connection(sockfd);
	} else {
		printf("Pentru a accesa biblioteca este necesara logarea!\n");
	}
}

// functia corepunzatoare comenzii get_books
void get_booksFunction(int *libraryCheck, char **cookies, char *jwt) {
	// aceasta comanda se poate realiza doar daca bibilioteca a fost accesata
	if (*libraryCheck) {
		int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			exit(1);
		}
		char *message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books",
			NULL, cookies, 1, jwt);
		send_to_server(sockfd, message);
		char *response = receive_from_server(sockfd);
		// lista de carti se afla la finalul response, incepand cu "["
		printf("%s\n", strstr(response, "["));
		close_connection(sockfd);
	} else {
		printf("Pentru a vizualiza cartile este necesara accesarea bibliotecii!\n");
	}
}

// functia corepunzatoare comenzii get_book
void get_bookFunction(int *checkLibrary, char **cookies, char *jwt) {
	// aceasta comanda se poate realiza doar daca biblioteca a fost accesata
	if (*checkLibrary) {
		int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			exit(1);
		}
		char *route = calloc(50, sizeof(char));
		if (!route) {
			exit(1);
		}
		int book_id = -1;

		printf("id=");
		scanf("%d", &book_id);
		
		sprintf(route, "%s/%d", "/api/v1/tema/library/books", book_id);

		// verific daca exista vreo carte cu id-ul cerut(adica la ruta de acces
		// creata mai sus)
		char *message = compute_get_request("34.241.4.235", route,
			NULL, cookies, 1, jwt);
		send_to_server(sockfd, message);
		char *response = receive_from_server(sockfd);
		// daca in response exista "No book was found!", inseamna ca nu exista nicio carte
		// cu id-ul cerut, formatul erorii fiind {"error":"No book was found!"}, altfel
		// cartea se afla la finalul response, incepand cu "["
		if (strstr(response, "No book was found!")) {
			printf("Id-ul pentru care a fost efectuata cererea este invalid!\n");
		} else {
			printf("%s\n", strstr(response, "["));
		}
		free(route);
		close_connection(sockfd);
	} else {
		printf("Pentru a vizualiza detalii despre carti este necesara accesarea bibliotecii!\n");
	}
}

// functia corepunzatoare comenzii add_book
void add_bookFunction(int *libraryCheck, char *jwt) {
	// aceasta comanda se poate realiza doar daca biblioteca a fost accesata
	if (*libraryCheck) {
		int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			exit(1);
		}
		char *title = calloc(50, sizeof(char));
		if (!title) {
			exit(1);
		}
		char *author = calloc(50, sizeof(char));
		if (!author) {
			exit(1);
		}
		char *genre = calloc(20, sizeof(char));
		if (!genre) {
			exit(1);
		}
		char *publisher = calloc(50, sizeof(char));
		if (!publisher) {
			exit(1);
		}
		int pages;

		printf("title=");
		scanf("%s", title);
		if (!strcmp(title, "")) {
			printf("Informatiile introduse nu respecta formatarea!\n");
			free(title);
			free(author);
			free(genre);
			free(publisher);
			close_connection(sockfd);
			return;
		}
		printf("author=");
		scanf("%s", author);
		if (!strcmp(author, "")) {
			printf("Informatiile introduse nu respecta formatarea!\n");
			free(title);
			free(author);
			free(genre);
			free(publisher);
			close_connection(sockfd);
			return;
		}
		printf("genre=");
		scanf("%s", genre);
		if (!strcmp(genre, "")) {
			printf("Informatiile introduse nu respecta formatarea!\n");
			free(title);
			free(author);
			free(genre);
			free(publisher);
			close_connection(sockfd);
			return;
		}
		printf("publisher=");
		scanf("%s", publisher);
		if(!strcmp(publisher, "")) {
			printf("Informatiile introduse nu respecta formatarea!\n");
			free(title);
			free(author);
			free(genre);
			free(publisher);
			close_connection(sockfd);
			return;
		}
		printf("pages=");
		scanf("%d", &pages);
		if(pages < 1) {
			printf("Informatiile introduse nu respecta formatarea!\n");
			free(title);
			free(author);
			free(genre);
			free(publisher);
			close_connection(sockfd);
			return;
		}

		char *pages_count = calloc(10, sizeof(char));
		if (!pages_count) {
			exit(1);
		}
		sprintf(pages_count, "%d", pages);

		JSON_Value *value = json_value_init_object();
		if (!value) {
			exit(1);
		}
		JSON_Object *object = json_value_get_object(value);
		if (!object) {
			exit(1);
		}
		json_object_set_string(object, "title", title);
		json_object_set_string(object, "author", author);
		json_object_set_string(object, "genre", genre);
		json_object_set_string(object, "page_count", pages_count);
		json_object_set_string(object, "publisher", publisher);
		
		char *form_data = json_serialize_to_string(value);
		char *message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json",
			&form_data, 1, NULL, 0, jwt);
		send_to_server(sockfd, message);
		receive_from_server(sockfd);

		printf("Cartea a fost adaugata cu succes!\n");
		free(title);
		free(author);
		free(genre);
		free(publisher);
		free(pages_count);
		json_value_free(value);
		close_connection(sockfd);
	} else {
		printf("Pentru a adauga o carte este necesara accesarea bibliotecii!\n");
	}
}

// functia corepunzatoare comenzii delete_book
void delete_bookFunction(int *libraryCheck, char **cookies, char *jwt) {
	// aceasta comanda se poate realiza doar daca biblioteca a fost accesata
	if (*libraryCheck) {
		int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			exit(1);
		}
		char *route = calloc(50, sizeof(char));
		if (!route) {
			exit(1);
		}
		int book_id = -1;

		printf("id=");
		scanf("%d", &book_id);
		
		sprintf(route, "%s/%d", "/api/v1/tema/library/books", book_id);
		
		// verificam daca id-ul corespunde unei carti
		char *message = compute_delete_request("34.241.4.235", route, NULL,
			cookies, 1, jwt);
		send_to_server(sockfd, message);
		char *response = receive_from_server(sockfd);
		// daca in response exista "No book was deleted!", inseamna ca nu exista nicio carte
		// cu id-ul cerut, formatul erorii fiind {"error":"No book was deleted!"}
		if (strstr(response, "No book was deleted!")) {
			printf("Id-ul pentru care a fost efectuata cererea este invalid!\n");
		} else {
			printf("Cartea a fost stearsa cu succes!\n");
		}
		free(route);
		close_connection(sockfd);
	} else {
		printf("Pentru a sterge cartea este necesara accesarea bibliotecii!\n");
	}
}

// functia corepunzatoare comenzii logout
void logoutFunction(int *loginCheck, int *libraryCheck, char **cookies, char *jwt) {
	// aceasta comanda se poate realizeaza doar daca a fost efectuata logarea
	if (*loginCheck) {
		int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			exit(1);
		}
		char *message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", 
			NULL, cookies, 1, jwt);
		send_to_server(sockfd, message);
		receive_from_server(sockfd);
		// setez loginCheck si libraryCheck la 0
		*loginCheck = 0;
		*libraryCheck = 0;
		printf("Deconectare realizata cu succes!\n");
	} else {
		printf("Nu este nimeni logat!\n");
	}
}

int main(int argc, char *argv[]) {
	int loginCheck = 0, libraryCheck = 0;
	int sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		exit(1);
	}
	char *jwt = calloc(BUFLEN, sizeof(char));
	if (!jwt) {
		exit(1);
	}
	char **cookies = calloc(1, sizeof(char *));
	if (!cookies) {
		exit(1);
	}
    cookies[0] = calloc(LINELEN, sizeof(char));
	if (!cookies[0]) {
		exit(1);
	}
	size_t len = 0;
	char *line = calloc(20, sizeof(char));
	if (!line) {
		exit(1);
	}

	while (1) {
		// preiau comanda de la tastatura
		char *command;
		getline(&line, &len, stdin);
		command = strtok(line, " ");
		if (command[strlen(command) - 1] == '\n') {
			command[strlen(command) - 1] = '\0';
		}
		if (command[strlen(command) - 1] == '\r') {
			command[strlen(command) - 1] = '\0';
		}

		// in functie de comanda primita apelez o alta functie care o realizeaza
		if (!strncmp(command, "register", 8)) {
			registerFunction();
		} else if (!strncmp(command, "login", 5)) {
			loginFunction(&loginCheck, &libraryCheck, cookies);
		} else if (!strncmp(command, "enter_library", 13)) {
			enter_libraryFunction(&loginCheck, &libraryCheck, cookies, jwt);
		} else if (!strncmp(command, "get_books", 9)) {
			get_booksFunction(&libraryCheck, cookies, jwt);
		} else if (!strncmp(command, "get_book", 8)) {
			get_bookFunction(&libraryCheck, cookies, jwt);
		} else if (!strncmp(command, "add_book", 8)) {
			add_bookFunction(&libraryCheck, jwt);
		} else if (!strncmp(command, "delete_book", 11)) {
			delete_bookFunction(&libraryCheck, cookies, jwt);
		} else if (!strncmp(command, "logout", 6)) {
			logoutFunction(&loginCheck, &libraryCheck, cookies, jwt);
		} else if (!strncmp(command, "exit", 4)) {
			break;
		}
	}

	free(*cookies);
	free(cookies);
	free(jwt);
	free(line);
	close_connection(sockfd);
	return(0);
}