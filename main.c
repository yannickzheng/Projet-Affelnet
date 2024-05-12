#include <stdio.h>
#include <stdlib.h>

#define NB_VOEUX 5
#define NB_ELEVE 5
#define NB_LYCEE 5

int lecture_eleve_suivant(FILE *f, int *eleve, int *score, int *lycees) {
	int n;
	int lyc = 0;
	n = fscanf(f, "%d,%d,", eleve, score);
	if(n == EOF) {
		return EOF;
	}
	for(int v = 0; v < NB_VOEUX; v++) {
		fscanf(f, "%d,", &lycees[lyc]);
		lyc++;
	}
	
	return 0;
}


int lecture_lycee_suivant(FILE *f, int *lycee, int *capacite) {
	return fscanf(f, "%d,%d", lycee, capacite);
}


struct eleve {
	int id;
	int score;
	int voeux[NB_VOEUX];
};


struct lycee {
	int id;
	int capacite;
	int capacite_actuel;
};


int cmpr_eleve(const void* e1, const void* e2) {
	return ((struct eleve*) e2)->score - ((struct eleve*) e1)->score;
}


struct lycee *recherche_lycee(struct lycee* lycees, int id) {
	int i = 0;
	while(i < NB_LYCEE) {
		if(lycees[i].id == id) {
			return &lycees[i];
		}
		i++;
	}
	return NULL;
}


struct couple_eleve_lycee {
	int eleve;
	int lycee;
};


int main(void) {
	FILE *f = fopen("voeux.csv", "r");
	struct eleve* eleves = malloc(NB_ELEVE*sizeof(struct eleve));
	for(int i = 0; i < NB_ELEVE; i++) {
		lecture_eleve_suivant(f, &(eleves[i].id), &(eleves[i].score), eleves[i].voeux);
		printf("%d %d ", eleves[i].id, eleves[i].score);
		for(int j = 0; j < NB_VOEUX; j++) {
			printf("%d ", eleves[i].voeux[j]);
		}
		printf("\n");
	}
	
	fclose(f);

	qsort(eleves, NB_ELEVE, sizeof(struct eleve), cmpr_eleve);
	
	for(int i = 0; i < NB_ELEVE; i++) {
		printf("%d\n",eleves[i].score);
	}
	
	f = fopen("lycees.csv", "r");

	struct lycee* lycees = malloc(NB_LYCEE*sizeof(struct lycee));
	
	for(int i = 0; i < NB_LYCEE; i++) {
		lecture_lycee_suivant(f, &(lycees[i].id), &(lycees[i].capacite));
		printf("%d %d\n", lycees[i].id, lycees[i].capacite);
		lycees[i].capacite_actuel = 0;
	}
	
	fclose(f);
	
	printf("\n\n");
	struct lycee *l = recherche_lycee(lycees, 1);
	printf("%d %d\n", l->id, l->capacite);

	struct couple_eleve_lycee* couples = malloc(NB_ELEVE*sizeof(struct couple_eleve_lycee));
	
	for(int i = 0; i < NB_ELEVE; i++) {
		struct lycee *lycee = recherche_lycee(lycees, eleves[i].voeux[0]);
		int j = 1;
		couples[i].lycee = -1;
		couples[i].eleve = eleves[i].id;
		while(j < NB_VOEUX && lycee->capacite_actuel >= lycee->capacite) {
			lycee = recherche_lycee(lycees, eleves[i].voeux[j]);
			j++;
		}
		if(NB_VOEUX != j) {
			couples[i].lycee = lycee->id;
			lycee->capacite_actuel++;
		}
		printf("%d %d\n", couples[i].eleve, couples[i].lycee);
	}

	printf("\n\n");

	for(int i = 0; i < NB_LYCEE; i++) {
		int id = lycees[i].id;
		printf("Lycée n°%d:\n", id);
		for(int j = 0; j < NB_ELEVE; j++) {
			if(couples[j].lycee == id) {
				printf("\t- %d\n", couples[j].eleve);
			}
		}
	}
	printf("Sans Lycée:\n");
	for(int i = 0; i < NB_ELEVE; i++) {
		if(couples[i].lycee == -1) {
			printf("\t- %d\n", couples[i].eleve);
		}
	}

	return 0;
}
	


