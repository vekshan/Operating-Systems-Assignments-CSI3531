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
	if (prcNum > 1)
	{
		int fd[2]; //0 -> read, 1 -> write    0 > stdin   1 > stdout
		pipe(fd);
		pid_t pid = fork();
		if (pid == 0)
		{					//child
                        
			close(fd[0]);	//  close the reading end of the pipe
			dup2(fd[1], 1); //replace stdout with writing end of the pipe
                        char prcNum_str[128]; // create an array containing prcNum
			snprintf(prcNum_str, 30, "%d", prcNum-1);
                        printf("prc_num is %d", prcNum_str[0]);	
			char *args[] = {"./cpr %d", prcNum_str, NULL};
                        write(fd[1], &prcNum , 10); // write the actual prcNum of the child into the pipe
                        close(fd[1]); // close the write end of the child
                       
			execv(args[0], args); 

		}
		else if (pid > 0)
		{ //parent
                        
                        close(fd[1]); // close the write end of the parent
                        printf("Processus %d commence\n", prcNum);                        
                        char output;
                        read(fd[0], &output, 10); // read from the pipe
                        //printf("output from pipe is %d", output);
                        printf("Processus %d termine\n", output);
                        
                        
                        close(fd[0]); // close the reading end of the pipe 
                        
    
                        
		}
		else
		{ //error
			fprintf(stderr, "fork() ou exec() non-reussi.\n");
		}
	}
	else{ //stop
             printf("Processus 1 commence\n");
             sleep(5);
             printf("Processus 1 termine\n");
	
	}
}
