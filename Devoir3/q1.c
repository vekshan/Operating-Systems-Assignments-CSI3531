#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

void *student_thread(void *param);
void *ta_thread(void *param);


int waiting_seats = 3;				  //number of seats availble in waiting room
pthread_mutex_t access_waiting_seats; //mutex to access waiting_seats
sem_t students_sem;					  // semaphore to notify Ta that student is ready
sem_t ta_sem;						  // semaphore to notify student that Ta is available

void *student_thread(void *param)
{
	int student_no = (intptr_t)param;

	int programming = (rand() % 5) + 1;//time spent programming

	printf("Student %d is programming for %d s.\n", student_no, programming);
	sleep(programming);

	//The while loop is exited only when the student has seen the TA
	while (1) { 
		pthread_mutex_lock(&access_waiting_seats);
		if (waiting_seats > 0) //if there are available seats
		{
			// request access for variable waiting_seats and decrement			

			printf("Student %d needs help and is at position %d .\n", student_no, 3 - waiting_seats);

			waiting_seats--;
			


			// notify TA, student is ready or wake him up

			sem_post(&students_sem);

			// release mutex 
			pthread_mutex_unlock(&access_waiting_seats);

			//wait for Ta to be available
			sem_wait(&ta_sem);

			printf("Student %d is is with Ta.\n", student_no);
			break;
		}
		else
		{ 
			//Release mutex now if waiting_seats <= 0
			pthread_mutex_unlock(&access_waiting_seats);
			//continue programming
			printf("Student %d will come back later.\n", student_no);
			programming = (rand() % 5) + 1;
			printf("Student %d is programming for %d s.\n", student_no, programming);
			sleep(programming);
		}
	}
	//terminate this threadonce student has seen the TA
		pthread_exit(NULL);

	

	
}

void *ta_thread(void *param)
{
	while (1)
	{
		//wait for student to be ready
		sem_wait(&students_sem);

		if (waiting_seats < 3) // if people are waiting
		{
			pthread_mutex_lock(&access_waiting_seats); // lock the mutex to access waiting students
			waiting_seats++; //increment waiting seats as an additional chair is now available in the waiting room
			sem_post(&ta_sem);				   //notify that Ta is available
			printf("TA is currently with a student, %d students in waiting room.\n", 3 - waiting_seats);
			pthread_mutex_unlock(&access_waiting_seats); // release the mutex

			
			sleep((rand() % 5) + 1);
		}
	}
}

int main(int argc, char* argv[])
{

	int numberOfStudents = 0; // number of students input by user
	if (argc < 2)
	{
		numberOfStudents = 6;
		//Use 6 students if not specified by user
		printf("Number of Students not specified. Using default (5) students.\n");
	}
	else
	{
		numberOfStudents = atoi(argv[1]);
		printf("Number of Students specified. Creating %d threads.\n", numberOfStudents);

	}

	pthread_t student[numberOfStudents];
	pthread_attr_t attr;
	pthread_t ta;

	srand(time(0)); //for random number

	//initialize mutex
	pthread_mutex_init(&access_waiting_seats, NULL);

	//initialize pthread attr
	pthread_attr_init(&attr);

	//initialize semaphores
	sem_init(&students_sem, 0, 1);
	sem_init(&ta_sem, 0, 1);

	
	
	//create student threads
	for (int i = 0; i < numberOfStudents ; i++)
	{
			pthread_create(&student[i], &attr, student_thread, (void *)((intptr_t)(i+1)));	
	}

	//create TA thread
	pthread_create(&ta, &attr, ta_thread, NULL);

	//wait for the threads to terminate
	for (int i = 0; i < numberOfStudents; i++)
	{
		pthread_join(student[i], NULL);
	}


	pthread_cancel(ta);
	pthread_mutex_destroy(&access_waiting_seats);
	sem_destroy(&students_sem);
	sem_destroy(&ta_sem);

}