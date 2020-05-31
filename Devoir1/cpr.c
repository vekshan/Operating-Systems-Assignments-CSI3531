/*------------------------------------------------------------
Fichier: cpr.c

Nom: Vekshan Bundhoo, Bhavika Sewpal
Numero d'etudiant: 300035157, 300089940

Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):
Le processus est un zombie car il a fini d'être exécuté mais il n'a pas été récolté par son parent.

	(s.v.p. completez cette partie);
Le processus, "[cpr] <defunct>" est un zombie car il a fini d'être exécuté mais il n'a pas été récolté (reaped) par son parent. 
Pour recolter le processus, le parent doit faire appel à wait(). L'appel à wait() n'a  pas été requis dans ce devoir car le parent 
se rendra compte qu'il ne peut plus lire le bout lecture du tuyau.
(Veuillez noter que je ai commenté le sleep(10) dans le code - ligne 119.)

-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

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
	int nbytes = 0;
	printf("Processus %d commence\n", prcNum);
	fflush(stdout);
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
			sprintf(prcNum_str, "%d", prcNum - 1);
			char *args[] = {"cpr", prcNum_str, NULL};
			execvp(args[0], args);
		}
		else
		{ //parent
			close(fd[1]);
			fflush(stdout);
			char readbuf[512];
            while((nbytes = read(fd[0], readbuf, 512)) > 0 ) {
				write(1, readbuf, nbytes);
			}
			//wait(NULL);
		}
	}
	//stop
	if (prcNum == 1)
	{
		sleep(5);
	}
	sleep(10);
	printf("Processus %d termine\n", prcNum);
	//sleep(10);
	fflush(stdout);
	close(1);
}
