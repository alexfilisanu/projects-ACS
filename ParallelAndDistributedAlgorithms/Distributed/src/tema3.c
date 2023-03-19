#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_COORDINATORS 4
#define MAX_WORKERS 20
#define ROOT 0

// functie ce afiseaza conexiunea specificata in enunt si realizeaza MPI_Send
int MPI_Send_with_log(int source, void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator) {
    printf("M(%d,%d)\n", source, destination);
    return MPI_Send(data, count, datatype, destination, tag, communicator);
}

// functie ce returneaza topologia in forma in care trebuie afisata
char* print_topology(int rank, int topology_counts[NUM_COORDINATORS], int topology[NUM_COORDINATORS][MAX_WORKERS]) {
    char* topology_str = malloc(128);
    if (!topology_str) {
        fprintf(stderr, "Eroare la alocarea memoriei!\n");
		exit(1);
    }
    sprintf(topology_str, "%d -> ", rank);

    for (int i = 0; i < NUM_COORDINATORS; i++) {
        char r[2];
        sprintf(r, "%d", i);
        strcat(topology_str, r);
        strncat(topology_str, ":", 1);

        for (int j = 0; j < topology_counts[i]; j++) {
            char r[2];
            sprintf(r, "%d", topology[i][j]);
            strcat(topology_str, r);
            if (j < topology_counts[i] - 1)
                strncat(topology_str, ",", 1);
        }

        strncat(topology_str, " ", 1);
    }

    return topology_str;
}

// functie ce trimite taskurile workerilor
void send_task_to_workers(int workers_number, int count, int steps, int rank, int start_pos, int* workers, int *V) {
    for (int i = 0; i < workers_number; i++) {
        int send_count;
        // calculez cate valori trebuie sa calculeze fiecare worker
        if (i == workers_number - 1) {
            send_count = count; 
        } else {
            send_count = steps;
            count -= send_count;
        }

        // trimit cate valori are de calculat un worker
        MPI_Send_with_log(rank, &send_count, 1, MPI_INT, workers[i], 0, MPI_COMM_WORLD);
        int start_pos_worker = start_pos + (i * steps);
        // trimit vectorul de la pozitia in care trebuie sa calculeze noile valori
        MPI_Send_with_log(rank, V + start_pos_worker, send_count, MPI_INT, workers[i], 0, MPI_COMM_WORLD);
    }
}

// functie ce primeste rezultatele de la workeri
void recive_result_from_workers(int workers_number, int count, int steps, int rank, int start_pos, int* workers, int* V) {
    for (int i = 0; i < workers_number; i++) {
        int send_count;
        if (i == workers_number - 1) {
            send_count = count; 
        } else {
            send_count = steps;
            count -= send_count;
        }

        // primesc rezultatele calculate de workeri si le pun in V la pozitia lor
        int start_pos_worker = start_pos + (int)(i * (int)steps);
        MPI_Recv(V + start_pos_worker, send_count, MPI_INT, workers[i], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int main (int argc, char *argv[]) {
    int numtasks, rank, N;

    N = atoi(argv[1]);

    int topology_workers_number[NUM_COORDINATORS];
    int topology[NUM_COORDINATORS][MAX_WORKERS];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // daca rankul este de tip COORDINATOR
    if (rank < NUM_COORDINATORS) {
        int workers_number;

        char *input_file_name = calloc(12, sizeof(char));
        if (!input_file_name) {
            fprintf(stderr, "Eroare la alocarea memoriei!\n");
		    exit(1);
        }
        sprintf(input_file_name, "cluster%d.txt", rank); 
        FILE *fptr = fopen(input_file_name, "r");
        if (!fptr) {
            fprintf(stderr, "Eroare la deschiderea fisierului %s!", input_file_name);
            exit(1);
        } 
        // citesc primul element ce reprezinta numarul total de numere din fisier
		fscanf(fptr, "%d", &workers_number);
        // creez vectorul de workeri(de dimensiune workers_number)
        int workers[workers_number];
		// citesc restul numerelor unul cate unul si le adaug in vectorul de workeri
		for (int i = 0; i < workers_number; i++) {
			fscanf(fptr, "%d", &workers[i]);
        }

        // pentru rankul curent actualizez valorile din topology_workers_number si topology
        topology_workers_number[rank] = workers_number;
        for (int i = 0; i < workers_number; i++) {
            topology[rank][i] = workers[i];
        }

        int master;
        // intr un for de la 0 la numarul maxim de coordonatori trimit pentru fiecare rank mereu la dreapta: rank ul, numarul 
        // de workeri si topologia workerilor (se opreste cand ce e in stanga ar trebui sa trimita din nou la el)
        // de exemplu: -> 0 trimite aceste informatii despre 0 lui 1, 1 trimite informatiile despre 0 lui 2, 2 lui 3
        //             -> 1 trimite aceste informatii despre 1 lui 2, 2 trimite informatiile despre 1 lui 3, 3 lui 0
        //             -> 2 trimite aceste informatii despre 2 lui 3, 3 trimite informatiile despre 2 lui 0, 0 lui 1
        //             -> 3 trimite aceste informatii despre 3 lui 0, 0 trimite informatiile despre 3 lui 1, 1 lui 2
        for (int i = 0; i < NUM_COORDINATORS; i++) {
            int curr = (rank + i) % NUM_COORDINATORS;
            int next = (rank + i + 1) % NUM_COORDINATORS;
            int prev = (rank + NUM_COORDINATORS - i - 1) % NUM_COORDINATORS; 
            // verificare pentru ca finalul inelului sa nu trimita informatia deja existenta catre inceputul acestuia
            if (next != rank) {
                // trimiterea rankului la dreapta
                MPI_Send_with_log(curr, &rank, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
                // primirea acestuia din stanga
                MPI_Recv(&master, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                // trimiterea numarului de workeri la dreapta 
                MPI_Send_with_log(curr, &topology_workers_number[rank], 1, MPI_INT, next, 0, MPI_COMM_WORLD);
                // primirea din stanga
                MPI_Recv(&topology_workers_number[master], 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // trimiterea topologiei workerilor la dreapta
                MPI_Send_with_log(curr, topology[rank], topology_workers_number[rank], MPI_INT, next, 0, MPI_COMM_WORLD);
                // primirea din stanga
                MPI_Recv(topology[master], topology_workers_number[master], MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        // in acest punct topologia pentru fiecare coodonator este completa si o afisez
        printf("%s\n", print_topology(rank, topology_workers_number, topology));

        // trimit topologia workerilor conectati direct
        for (int i = 0; i < workers_number; i++) {
            MPI_Send_with_log(rank, &rank, 1, MPI_INT, workers[i], 0, MPI_COMM_WORLD);
            for (int j = 0; j < NUM_COORDINATORS; j++) {
                MPI_Send_with_log(rank, &topology_workers_number[j], 1, MPI_INT, workers[i], 0, MPI_COMM_WORLD);
                MPI_Send_with_log(rank, topology[j], topology_workers_number[j], MPI_INT, workers[i], 0, MPI_COMM_WORLD);
            }
        }

        // calculez in steps numarul aproximativ de calcule pe care trbuie sa l faca fiecare worker
        int total_workers_number = 0;
        for (int i = 0; i < NUM_COORDINATORS; i++) {
            total_workers_number += topology_workers_number[i];
        }
        float steps = (float)N / total_workers_number;
    
        int* V;
        int count, start_pos = 0;
        // daca rank ul este ROOT 
        if (rank == ROOT) {
            // creez vectorul initial si il initializez cu valorile cerute
            V = malloc(N * sizeof(int));
            if (!V) {
                fprintf(stderr, "Eroare la alocarea memoriei!\n");
		        exit(1);
            }
            for (int i = 0; i < N; i++) {
                V[i] = N - i - 1;
            }

            // calculez cate operatii trebuie sa faca in total workerii acestui rank
            count = topology_workers_number[rank] * steps;
            // trimit workerilor operatiile care trebuie facute si primesc de la acestia rezultatele  
            send_task_to_workers(workers_number, count, steps, rank, start_pos, workers, V);
            recive_result_from_workers(workers_number, count, steps, rank, start_pos, workers, V);
            
            // trimit la dreapta(in inel) pozitia de la care trebuie modificate valori si vectorul de valori
            start_pos = topology_workers_number[0] * steps;
            MPI_Send_with_log(rank, &start_pos, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Send_with_log(rank, V, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        } else {
            if (rank == 1) {
                // primesc de la stanga(in inel) pozitia de la care trebuie modificate valori si vectorul de valori
                MPI_Recv(&start_pos, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                V = malloc(N * sizeof(int));
                if (!V) {
                    fprintf(stderr, "Eroare la alocarea memoriei!\n");
                    exit(1);
                }
                MPI_Recv(V, N, MPI_INT, ROOT, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                // calculez cate operatii trebuie sa faca in total workerii acestui rank
                count = topology_workers_number[rank] * steps;
                // trimit workerilor operatiile care trebuie facute si primesc de la acestia rezultatele  
                send_task_to_workers(workers_number, count, steps, rank, start_pos, workers, V);
                recive_result_from_workers(workers_number, count, steps, rank, start_pos, workers, V);
                
                // trimit la dreapta(in inel) pozitia de la care trebuie modificate valori si vectorul de valori
                start_pos += topology_workers_number[1] * steps;
                MPI_Send_with_log(rank, &start_pos, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
                MPI_Send_with_log(rank, V, N, MPI_INT, 2, 0, MPI_COMM_WORLD);
            } else {
                if (rank == 2) {
                    // primesc de la stanga(in inel) pozitia de la care trebuie modificate valori si vectorul de valori
                    MPI_Recv(&start_pos, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    V = malloc(N * sizeof(int));
                    if (!V) {
                        fprintf(stderr, "Eroare la alocarea memoriei!\n");
                        exit(1);
                    }
                    MPI_Recv(V, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    
                    // calculez cate operatii trebuie sa faca in total workerii acestui rank
                    count = topology_workers_number[rank] * steps;
                    // trimit workerilor operatiile care trebuie facute si primesc de la acestia rezultatele  
                    send_task_to_workers(workers_number, count, steps, rank, start_pos, workers, V);
                    recive_result_from_workers(workers_number, count, steps, rank, start_pos, workers, V);
                    
                    // trimit la dreapta(in inel) pozitia de la care trebuie modificate valori si vectorul de valori
                    start_pos += topology_workers_number[2] * steps;
                    MPI_Send_with_log(rank, &start_pos, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
                    MPI_Send_with_log(rank, V, N, MPI_INT, 3, 0, MPI_COMM_WORLD);
                } else {
                    // primesc de la stanga(in inel) pozitia de la care trebuie modificate valori si vectorul de valori
                    MPI_Recv(&start_pos, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    V = malloc(N * sizeof(int));
                    if (!V) {
                        fprintf(stderr, "Eroare la alocarea memoriei!\n");
                        exit(1);
                    }
                    MPI_Recv(V, N, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    
                    // calculez cate operatii trebuie sa faca in total workerii acestui rank
                    count = N - (int)(topology_workers_number[0] * steps) - (int)(topology_workers_number[1] * steps) - 
                        (int)(topology_workers_number[2] * steps);
                    // trimit workerilor operatiile care trebuie facute si primesc de la acestia rezultatele  
                    send_task_to_workers(workers_number, count, steps, rank, start_pos, workers, V);
                    recive_result_from_workers(workers_number, count, steps, rank, start_pos, workers, V);
                    
                    // trimit la dreapta(in inel), adica lui ROOT vectorul completat
                    MPI_Send_with_log(rank, V, N, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
                }
            }
        }
        
        // procesul root afiseaza vectorul primit din stanga
        if (rank == ROOT) {
            MPI_Recv(V, N, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rezultat: ");
            for (int i = 0; i < N; i++) {
                printf("%d ", V[i]);
            }
        }

        // daca rankul este de tip WORKER
    } else {
        int master;
        // in master salvez parintele workerului
        MPI_Recv(&master, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
        // primesc de la acesta topologia completa si o afisez
        for (int i = 0; i < NUM_COORDINATORS; i++) {
            MPI_Recv(&topology_workers_number[i], 1, MPI_INT, master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(topology[i], topology_workers_number[i], MPI_INT, master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        printf("%s\n", print_topology(rank, topology_workers_number, topology));

        // primesc numarul de operatii care trebuie facute si vectorul in care trebuie
        int count;
        MPI_Recv(&count, 1, MPI_INT, master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int *V = malloc(count * sizeof(int));
        if (!V) {
            fprintf(stderr, "Eroare la alocarea memoriei!\n");
            exit(1);
        }
        MPI_Recv(V, count, MPI_INT, master, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // realizez operatia ceruta
        for (int i = 0; i < count; i++) {
            V[i] *= 5;
        }

        // trimit catre master vectorul cu valorile modificate
        MPI_Send_with_log(rank, V, count, MPI_INT, master, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}