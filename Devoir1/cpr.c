/*------------------------------------------------------------
Fichier: cpr.c

Nom: Vekshan Bundhoo, 
Numero d'etudiant: 300035157, 

Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):

	(s.v.p. completez cette partie);

-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

/* Prototype */
void creerEnfantEtLire(int);

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
	int numeroProcessus;

	if (ac == 2)
	{
		if (sscanf(av[1], "%d", &numeroProcessus) == 1)
		{
			creerEnfantEtLire(numeroProcessus);
		}
		else
			fprintf(stderr, "Ne peut pas traduire argument\n");
	}
	else
		fprintf(stderr, "Arguments pas valide\n");
	return (0);
}

/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{

	/* S.V.P. completez cette fonction selon les
       instructions du devoirs. */
	char buf[32];
	snprintf(buf, sizeof(buf), "Processus %d commence\n", prcNum);
	write(1, buf, strlen(buf));
	if (prcNum > 1)
	{
		int fd[2]; //0 -> read, 1 -> write
		pipe(fd);
		int pid;
		if ((pid = fork()) == -1)
		{
			perror("fork() non-reussi.\n");
			exit(1);
		}
		if (pid == 0)
		{					//child
			close(fd[0]);	// fermer l'entree du tuyau - non utilisee
			dup2(fd[1], 1); //remplacer la sortie standard avec la sortie du tuyau
			char prcNum_str[32];
			snprintf(prcNum_str, sizeof(prcNum_str), "%d", prcNum - 1);
			char *args[] = {"cpr", prcNum_str, NULL};
			execvp(args[0], args);
		}
		else
		{ //parent
		}
	}
	//stop
}
