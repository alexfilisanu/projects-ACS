#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define MAX_NAME_SIZE 100

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

/* elibereaza toata memoria alocata dinamic in program incepand cu directorul  
 * target; apelat din main() cu parametrul "home" elibereaza toata memoria 
 * alocata in program;
 * +++ se apeleaza recursiv pentru fiecare director
 */
void stop (Dir* target) 
{
	Dir *d = target->head_children_dirs;
	while (d) {	
		Dir *aux = d->next;
		stop(d);
		d = aux;
	}

	File *f = target->head_children_files;
	while (f) {
		File *aux = f->next;
		free(f->name);
		free(f);
		f = aux;
	}

	free(target->name);
	free(target);
}

// creeaza un fisier cu numele "name" in directorul "parent" 
void touch (Dir* parent, char* name) 
{
	File *file = malloc(sizeof(File));
	if (!file)
		exit(1);
	file->name = malloc(MAX_NAME_SIZE);
	if (!file->name)
		exit(1);
	strcpy(file->name, name);
	file->parent = parent;
	file->next = NULL;
	
	/* daca parent->head_children_files nu exista => fisierul nou devine
	 * parent->head_children_files; 
	 * altfel, adaug fisierul nou la finalul listei (daca deja exista eliberez
	 * memoria alocata precedent)
	 */
	if (!file->parent->head_children_files) {
		file->parent->head_children_files = file;
		return;
	}
	File *f = file->parent->head_children_files;
	while (f) {
		if (!strcmp(f->name, file->name)) {
			printf("File already exists\n");
			free(file->name);
			free(file);
			return;
		}
		if (!f->next) {
			f->next = file;
			return;
		}
		f = f->next;
	}
}

// creeaza un director cu numele "name" in directorul "parent"
void mkdir (Dir* parent, char* name) 
{
	Dir *dir = malloc(sizeof(Dir));
	if (!dir)
		exit(1);
	dir->name = malloc(MAX_NAME_SIZE);
	if (!dir->name)
		exit(1);
	strcpy(dir->name, name);
	dir->parent = parent;
	dir->head_children_files = NULL;
	dir->head_children_dirs = NULL;
	dir->next = NULL;

	/* daca parent->head_children_dirs nu exista => directorul nou devine
	 * parent->head_children_dirs; 
	 * altfel, adaug directorul nou la finalul listei (daca deja exista 
	 * eliberez memoria alocata precedent)
	 */
	if (!dir->parent->head_children_dirs) {
		dir->parent->head_children_dirs = dir;
		return;
	}
	Dir *d = dir->parent->head_children_dirs;
	while (d) {
		if (!strcmp(d->name, dir->name)) {
			printf("Directory already exists\n");
			free(dir->name);
			free(dir);
			return;
		}
		if (!d->next) {
			d->next = dir;
			return;
		}
		d = d->next;
	}
}

// afiseaza toate directoarele, respectiv fisierele din directorul "parent"
void ls (Dir* parent) 
{
	Dir *d = parent->head_children_dirs;
	while (d) {
		printf("%s\n", d->name);
		d = d->next;
	}

	File *f = parent->head_children_files;
	while (f) {
		printf("%s\n", f->name);
		f = f->next;
	}
}

// elimina fisierul cu numele "name" din directorul "parent" 
// ++ elibereaza memoria
void rm (Dir* parent, char* name) 
{
	File *f = parent->head_children_files, *ant = NULL;
	for (; f != NULL; ant = f, f = f->next) {
		if (!strcmp(f->name, name)) {
			// extragere de la inceputul listei
			if (!ant)
				parent->head_children_files = f->next;
			// extragere din alt loc
			else
				ant->next = f->next;

			free(f->name);
			free(f);
			return;
		}
	}
	// daca fisierul nu a fost gasit
	printf("Could not find the file\n");
}

// elimina directorul cu numele "name" din directorul "parent" si toate 
// fisierele care se gasesc in acesta ++ elibereaza memoria (functia stop)
void rmdir (Dir* parent, char* name) {
	Dir *d = parent->head_children_dirs, *ant = NULL;
	for (; d != NULL; ant = d, d = d->next) {
		if (!strcmp(d->name, name)) {
			//extragere de la inceputul listei
			if (!ant)
				parent->head_children_dirs = d->next;
			// extragere din alt loc
			else
				ant->next = d->next;

			// se apeleaza recursiv pentru eliberarea intregii memorii
			// ocupate in interiorul directorului
			stop(d);
			return;
		}
	}
	// daca directorul nu a fost gasit
	printf("Could not find the dir\n");
}

// modifica directorul "target" in directorul cu numele "name"
void cd(Dir** target, char *name)
{
	// ".." => directorul parinte
	if (!strcmp(name, "..")) {
		if ((*target)->parent)
			*target = (*target)->parent;
		return;
	}

	Dir *d = (*target)->head_children_dirs;
	while (d) {
		if (!strcmp(d->name, name)) {
			*target = d;
			return;
		}
		d = d->next;
	}
	// am ajuns la final => directorul nu a fost gasit
	printf("No directories found!\n");
}

/* intoarce un sir de caractere ce reprezinta calea de la directorul
 * radacina("home") pana la directorul "target";
 * creez sirul returnat("str") cu ajutorul "sprintf" si a unui buffer
 * pe care il dezaloc la finalul acestei functii
 */
char *pwd (Dir* target) 
{
	char *str = malloc(MAX_NAME_SIZE);
	if (!str)
		exit(1);
	char *buffer = malloc(MAX_NAME_SIZE);
	if (!buffer)
		exit(1);

	while (target) {
		sprintf(buffer, "%s", str);
		sprintf(str, "/%s%s", target->name, buffer);

		target = target->parent;
	}
	free (buffer);
	return str;
}

/* afiseaza interiorul directorului "target" intr-o forma arborescenta si
 * al fiecarului director din interior ++ apelare recursiva pentru fiecare 
 * director in care incrementez "level"
 */
void tree (Dir* target, int level) 
{
	Dir *d = target->head_children_dirs;
	while (d) {
		for (int i = 0; i < level; i++)
			printf("    ");

		printf("%s\n", d->name);
		tree(d, level + 1);
		d = d->next;
	}

	File *f = target->head_children_files;
	while (f) {
		for (int i = 0; i < level; i++)
			printf("    ");
		
		printf("%s\n", f->name);
		f = f->next;
	}
}

/* schimba numele "oldname" din directorul "parent" in "newname";
 * pastrez o referinta catre structura pe care o elimin temporar din lista
 * si o adaug la finalul acesteia cu numele schimbat
 */
void mv(Dir* parent, char *oldname, char *newname) 
{
	void *p = NULL;
	Dir *d = parent->head_children_dirs, *ant = NULL;
	File *f = parent->head_children_files, *antf = NULL;

	// verific ca numele "newname" sa nu fie deja folosit
	while (d) {
		if (!strcmp(d->name, newname)) {
			printf("File/Director already exists\n");
			return;
		}
		d = d->next;
	}
	while(f) {
		if (!strcmp(f->name, newname)) {
			printf("File/Director already exists\n");
			return;
		}
		f = f->next;
	}

	d = parent->head_children_dirs;
	for (; d != NULL; ant = d, d = d->next)	{
		if (!strcmp(d->name, oldname)) {
			// p retine datele directorului pe care il mut la finalul listei
			p = (void *)d;
			// extragere de la inceputul listei
			if (!ant) {
				// este singurul element al listei => modific direct numele
				if (!d->next) {
					strcpy(((Dir*)d)->name, newname);
					return;
				}

				parent->head_children_dirs = d->next;
				d = parent->head_children_dirs;
			}// extragere din alt loc
			else {
				ant->next = d->next;
				d = ant->next;
			}

			strcpy(((Dir*)p)->name, newname);
			((Dir*)p)->next = NULL;
		}
	}

	// daca p exista => "newname" a fost gasit => adaug elementul la final
	if (p) {
		ant->next = (Dir *)p;
		return;
	}

	f = parent->head_children_files;
	for (; f != NULL; antf = f, f = f->next) {
		if (!strcmp(f->name, oldname)) {
			// p retine datele fisierului pe care il mut la finalul listei
			p = (void *)f;
			//extragere de la inceputul listei
			if (!antf) {
				// este singurul element al listei => modific direct numele
				if (!f->next) {
					strcpy(((File*)f)->name, newname);
					return;
				}
	
				parent->head_children_files = f->next;
				f = parent->head_children_files;
			}// extragere din alt loc
			else {
				antf->next = f->next;
				f = antf->next;
			}

			strcpy(((File *)p)->name, newname); 
			((File *)p)->next = NULL;
		}
	}

	// daca p exista => "newname" a fost gasit => adaug elementul la final
	if (p) {
		antf->next = (File *)p;
		return;
	}

	// daca se ajunge aici => elemntul nu a fost gasit
	printf("File/Director not found\n");
}

int main ()
{
	char *line = NULL;
	size_t len = 0;
	char *comand, *name;

	// aloc memorie si creez directotul "home"
	Dir *home = malloc(sizeof(Dir));
	if (!home)
		exit(1);
	home->name = malloc(MAX_NAME_SIZE);
	if (!home->name)
		exit(1);
	strcpy(home->name, "home");
	home->parent = NULL;
	home->head_children_files = NULL;
	home->head_children_dirs = NULL;
	home->next = NULL;

	// current va pastra intotdeauna directorul curent
	Dir *current = home;

	// cat timp citesc comenzi in afara de "stop"
	do {
		getline(&line, &len, stdin);
		comand = strtok(line, " ");		
		if (comand[strlen(comand) - 1] == '\n')
            comand[strlen(comand) - 1] = '\0';
        if (comand[strlen(comand) - 1] == '\r')
            comand[strlen(comand) - 1] = '\0';

		name = strtok(NULL, " ");
		if (name) {
			if (name[strlen(name) - 1] == '\n')
    	        name[strlen(name) - 1] = '\0';
       		if (name[strlen(name) - 1] == '\r')
            	name[strlen(name) - 1] = '\0';
		}
		
		if (!strcmp(comand, "touch"))
			touch(current, name);

		if (!strcmp(comand, "mkdir"))
			mkdir(current, name);

		if (!strcmp(comand, "ls"))
			ls(current);

		if (!strcmp(comand, "rm"))
			rm(current, name);

		if (!strcmp(comand, "rmdir"))
			rmdir(current, name);

		if (!strcmp(comand, "cd"))
			cd(&current, name);

		if (!strcmp(comand, "tree"))
			tree(current, 0);

		if (!strcmp(comand, "pwd")) {
			char *str = pwd(current);
			printf("%s\n", str);
			free(str);
		}

		if (!strcmp(comand, "mv")) {
			char *name2 = strtok(NULL, " ");
			if (name2) {	
			if (name2[strlen(name2) - 1] == '\n')
    	        name2[strlen(name2) - 1] = '\0';
       		if (name2[strlen(name2) - 1] == '\r')
            	name2[strlen(name2) - 1] = '\0';
			} 

			mv(current, name, name2);
		}

	} while (strcmp(comand, "stop"));

	stop(home);
	free(line);

	return 0;
}
