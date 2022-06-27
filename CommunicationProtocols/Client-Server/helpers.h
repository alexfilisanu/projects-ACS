#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <map>
#include <list>
#include <string.h>

#define BUFLEN 1500
#define MAX_CLIENTS 5

#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
				__FILE__, __LINE__);		\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while(0)	

// mesaj de tip UDP
struct UDPmessage {
	char topic[50];
	char type;
	char data[BUFLEN];
};

// structura pentru trimiterea unui request de catre clienti catre server
struct send_request {
	char ID[10];               
	int type;                  
	char topic[50];            
	int flag;                  
};
