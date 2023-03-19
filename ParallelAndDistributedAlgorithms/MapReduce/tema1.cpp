#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <queue>
#include <filesystem>
#include <string.h>
#include <bits/stdc++.h>
#include <set>
#include <vector>

pthread_barrier_t barrier;
pthread_mutex_t mutex;

struct my_arg {
	int thread_id; // numarul thread ului
	int nr_mappers;
	int nr_reducers;
	char *dir_name; // numele directorului pe care il folosesc la crearea fisierului in care scriu
	std::queue<char *> *files_queue; // coada cu numele fisierelor ce trebuie deschise
	std::vector<std::set<int>> *vectorOfSet; // vector in care stochez valorile ce sunt puteri 
											 // perfecte sub forma unor set uri
};

// functie ce retuneaza daca un numar este putere perfecta;
// pentru a calcula asta intr un mod eficient fac o cautare
// binara recursiva in care compar pow(mid, exp) cu numarul
bool isPower(int nr, int start, int last, int exp)
{
	if (start > last) {
		return false;
	}
	int mid = (start + last) / 2;
	if (pow (mid, exp) == nr) {
		return true;
	}
	if (pow (mid, exp) > nr) {
		return isPower(nr, start, mid - 1, exp);
	} else {
		return isPower(nr, mid + 1, last, exp);
	}
}

void *f(void *arg)
{
	int thread_id = (*(struct my_arg *)arg).thread_id;
	int nr_mappers = (*(struct my_arg *)arg).nr_mappers;
	int nr_reducers = (*(struct my_arg *)arg).nr_reducers;
	char *dir_name = (*(struct my_arg *)arg).dir_name; 
	std::queue<char *> *files_queue = (*(struct my_arg *)arg).files_queue;
	std::vector<std::set<int>> *vectorOfSet = (*(struct my_arg *)arg).vectorOfSet;
	
	// daca thread ul este de tip map
	if (thread_id < nr_mappers) {	
		FILE *fptr = NULL;

		// folosind un mutex deschid primul fisier din coada, pe care apoi il scot
		pthread_mutex_lock(&mutex);
		if (!files_queue->empty()) {
			fptr = fopen(files_queue->front(), "r");
			if (!fptr) {
				fprintf(stderr, "Eroare la deschiderea fisierului %s!\n", files_queue->front());
				exit(1);
			}
			files_queue->pop();
		}
		pthread_mutex_unlock(&mutex);
		
		// cat timp un fisier a fost deschis incep prelucrarea elementelor 
		// din interiorul acestuia
		while (fptr != NULL) {
		    int x, nr;
			// citesc primul element ce reprezinta numarul total de numere din fisier
			fscanf(fptr, "%d", &nr);
			// citesc restul numerelor unul cate unul
			for (int i = 0; i < nr; i++) {
				fscanf(fptr, "%d", &x);
				// verific pentru toti exponentii daca numarul citit este a ^ (exp + 2) 
				for (int exp = 0; exp < nr_reducers; exp++) {
				 	// daca este, il adaug in set ul de pe pozitia exp a vectorului;
					// folosesc set uri pentru a nu adauga duplicate, pe care ulterior
					// ar fi trebuit sa le elimin 
					if (isPower(x, 1, x, exp + 2)) {
						vectorOfSet->at(exp).insert(x);
				 	}
				}
			}
			
			// inchid fisierul prelucrat si ii asignez pointer ului fptr NULL pentru 
			// a iesi din while daca thread ul nu o sa deschida un nou fisier
			fclose(fptr);
			fptr = NULL;
			
			// folosind un mutex, incerc deschiderea unui nou fisier (acest lucru este 
			// posibil doar daca coada de fisiere nu este goala; 
			// thread ul, fiind mai rapid, termina deja prelucrarea unui fisier si poate
			// sa inceapa prelucrarea unui nou fisier)
			pthread_mutex_lock(&mutex);
			if (!files_queue->empty()) {
				fptr = fopen(files_queue->front(), "r");
				if (!fptr) {
					fprintf(stderr, "Eroare la deschiderea fisierului %s!\n", files_queue->front());
					exit(1);
				}
				files_queue->pop();
			}
			pthread_mutex_unlock(&mutex);
		}
	}

	// sincronizare folosind bariera - acest punct este depasit doar in momentul
	// in care toate thread urile de tip map si au incheiat executia
	pthread_barrier_wait(&barrier);

	// daca thread ul este de tip reduce
	if (thread_id >= nr_mappers) {
	  	char *out_name = (char*)malloc(20);
		if (!out_name) {
			fprintf(stderr, "Eroare la alocarea memoriei!\n");
			exit(1); 
		}
		// creez numele fisierului in care urmeaza sa scriu rezultaul 
	  	sprintf(out_name, "%s/out%d.txt", dir_name, thread_id - nr_mappers + 2);
		FILE* fptr = NULL;
		fptr = fopen (out_name, "w");
		if (!fptr) {
			fprintf(stderr, "Eroare la deschiderea fisierului %s\n!", out_name);
			exit(1);
		}
		// rezultatul pe care il scriu este marimea set ului de la indicele
		// (thread_id - nr_mappers) al vectorului
		fprintf(fptr, "%ld", vectorOfSet->at(thread_id - nr_mappers).size());
		fclose(fptr);
		free(out_name);	
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	if (argc != 4) {
		fprintf(stderr, "Numar invalid de parametri!");
		exit(1);
	}

	int nr_mappers, nr_reducers;
	nr_mappers = atoi(argv[1]);
	nr_reducers = atoi(argv[2]);

	std::queue<char*> files_queue;
	std::vector<std::set<int>> mapOfSet(nr_reducers);
	std::vector<int> result(nr_reducers);
	pthread_t *threads = (pthread_t *) malloc((nr_mappers + nr_reducers) * sizeof(pthread_t));
	if (!threads) {
		fprintf(stderr, "Eroare la alocarea memoriei!\n");
		exit(1); 
	}
	struct my_arg *arguments = (struct my_arg*) malloc((nr_mappers + nr_reducers) * sizeof(struct my_arg));
	if (!arguments) {
		fprintf(stderr, "Eroare la alocarea memoriei!\n");
		exit(1); 
	}
	
	FILE *fptr = fopen(argv[3], "r");
	if (!fptr) {
		fprintf(stderr, "Eroare la deschiderea fisierului %s!", argv[3]);
		exit(1);
	}
	
	int nr;
	// citesc primul element ce reprezinta numarul total de stringuri din fisier
	fscanf(fptr, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		char *str = (char *)malloc(sizeof(char*));
	 	if (!str) {
			fprintf(stderr, "Eroare la alocarea memoriei!\n");
			exit(1); 
		} 
		fscanf(fptr, "%s", str);
		// adaug numele fisierelor citite in coada fisierelor
		files_queue.push(str);
	}
	fclose(fptr);
	
	// initializare mutex si bariera
	pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barrier, NULL, nr_mappers + nr_reducers);

	int r;
	void *status;
	// pastrez doar numele directorului din argv[3] pentru a-l trimite
	// ca argument
	char *dir_name = strtok(argv[3], "/");
	// creez toate thread urile in acelasi for
	for (int i = 0; i < nr_mappers + nr_reducers; i++) {
		arguments[i].thread_id = i;
		arguments[i].nr_mappers = nr_mappers;
		arguments[i].nr_reducers = nr_reducers;
		arguments[i].dir_name = dir_name;
		arguments[i].files_queue = &files_queue;
		arguments[i].vectorOfSet = &mapOfSet;
		
		r = pthread_create(&threads[i], NULL, f, &arguments[i]);
		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

 	// pornesc toate thread urile in acelasi for
	for (int i = 0; i < nr_mappers + nr_reducers; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	//  distrugerea mutex ului si a barierei
	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);

	free(threads);
	free(arguments);
	
	return 0;
}
