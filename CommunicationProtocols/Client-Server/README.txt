Filisanu Mihai-Alexandru
324CB

    Am ales sa folosesc ca limbaj C++ pentru a folosi implementarile de ista si map
deja existente. Am folosit ca schelet pentru tema rezolvarea laboratorului 8 (TCP
si multiplexare I/O).

    In helpers.h am pastrat definirea macro urilor DIE, BUFLEN si MAX_CLIENTS din 
labortor si am definit structurile UDPmessage si send_request, folosita pentru 
trimiterea unui request de catre clienti catre server.  

    In server.cpp am definit 2 structuri Client, ce contine un bool care retine daca
clientul e online sau nu, un int pentru socket si o lista de char* pentru post uri.
Cealalta structura, Topic, retine ID ul si SF ul. Am creat si o functie getPost, ce
returneaza informatiile din payload in format human-readable, in functie de tipul
acestora(INT, SHORT_REAL, FLOAT sau STRING). 
    La inceput deschid un socket TCP si unul UDP. Definesc 2 map uri: clients ce 
are drept key ID ul clientului si drept value o structura de tip client si topics
ce are drept key un topic(string) si ca value o lista de structuri Topic. Folosesc
aceste structuri pentru a nu fi nevoit sa parcurg de fiecare data cate o lista pana
gasesc ce cautam. Daca am primit comanda "exit" ma opresc si deconectez clientii.
Daca am primit un mesaj pe socketul UDP preiau mesajul folosind functia definita 
anterior(getPost) si daca clientul e activ il trimit, iar daca nu, dar SF == 1 il 
stochez. Daca am primit un mesaj pe socketul TCP, serverul trebuie sa il accepte, 
iar in alt caz(socketul de client) serverul trebuie sa le receptioneze. In functie 
de tipul cererii(IDENTIFY, SUBSCRIBE, UNSUBSCRIBE, DISCONNECT) indeplinesc sarcinile
respective(prezentate in comentarii).

    In subscriber.cpp deschid socketul TCP si creez o conexiune cu serverul. Dezactivez
algoritmul lui Nagle. Daca mesajul e primit de la stdin trimite in functie de tipul 
acestuia mesajul corespunzator serverului. Daca mesajul este de la server verific 
daca este de tipul DISCONNECT, caz in care ma opresc, sau in string, caz in care il 
afisez.

    Output dupa rularea comenzii "sudo python3 test.py": 
rm -f server subscriber
Compiling
g++ -Wall server.cpp -o server
g++ -Wall subscriber.cpp -o subscriber
Starting the server
Starting subscriber C1
Generating one message for each topic
Subscribing C1 to all topics without SF
Generating one message for each topic
Disconnecting subscriber C1
Generating one message for each topic
Starting subscriber C1
Starting another subscriber with ID C1
Starting subscriber C2
Subscribing C2 to topic a_non_negative_int without SF
Subscribing C2 to topic a_negative_int with SF
Generating one message for topic a_non_negative_int
Generating one message for topic a_negative_int
Disconnecting subscriber C2
Generating one message for topic a_non_negative_int
Generating three messages for topic a_negative_int
Starting subscriber C2
Generating one message for each topic 30 times in a row
Stopping the server
rm -f server subscriber

RESULTS
-------
compile...........................passed
server_start......................passed
c1_start..........................passed
data_unsubscribed.................passed
c1_subscribe_all..................passed
data_subscribed...................passed
c1_stop...........................passed
c1_restart........................passed
data_no_clients...................passed
same_id...........................passed
c2_start..........................passed
c2_subscribe......................passed
c2_subscribe_sf...................passed
data_no_sf........................passed
data_sf...........................passed
c2_stop...........................passed
data_no_sf_2......................passed
data_sf_2.........................passed
c2_restart_sf.....................passed
quick_flow........................passed
server_stop.......................passed

    In cazul unor rulari repetate primesc, uneori, eroarea BrokenPipeError sau eroare
la pornirea serverului, desi daca il pornesc manual totul functioneaza.