#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>		

pthread_t *Students;		
pthread_t TA;			
int CCount = 0;
int CurrentIndex = 0;
sem_t TA_Sleep;
sem_t SSem;
sem_t CSem[3];
pthread_mutex_t CAccess;
void *TA_Act();
void *Std_Act(void *threadID);
int main(int argc, char* argv[])
{
	int nos;		
	int id;
	srand(time(NULL));
	sem_init(&TA_Sleep, 0, 0);
	sem_init(&SSem, 0, 0);
	for(id = 0; id < 3; ++id)			
	sem_init(&CSem[id], 0, 0);
	pthread_mutex_init(&CAccess, NULL);
	if(argc<2)
	{
		printf("No. of students are not specified. Using default number of students (5).\n");
		nos = 5;
	}
	else
	{
printf("No. of students are specified. So Creating %d threads.\n",        nos);
		nos = atoi(argv[1]);
	}
	Students = (pthread_t*) malloc(sizeof(pthread_t)*nos);
	pthread_create(&TA, NULL, TA_Act, NULL);	
	for(id = 0; id < nos; id++)
	pthread_create(&Students[id], NULL, Std_Act,(void*) (long)id);
	pthread_join(TA, NULL);
	for(id = 0; id < nos; id++)
	pthread_join(Students[id], NULL);
	free(Students); 
	return 0;
}

void *TA_Act()
{
	while(1)
	{
		sem_wait(&TA_Sleep);		
		printf("Students awakened TA\n");
		while(1)
		{
			pthread_mutex_lock(&CAccess);
			if(CCount == 0) 
			{
			
				pthread_mutex_unlock(&CAccess);
				break;
			}			
			sem_post(&CSem[CurrentIndex]);
			CCount--;
			printf("Student left his/her chair. Remaining Chairs %d\n", 3 - CCount);
			CurrentIndex = (CurrentIndex + 1) % 3;
			pthread_mutex_unlock(&CAccess);
			printf("\t TA at present is  helping the student.\n");
			sleep(5);
			sem_post(&SSem);
			usleep(1000);
		}
	}
}
void *Std_Act(void *threadID) 
{
	int ProgrammingTime;
	while(1)
	{
		printf("Student %ld is doing programming assignment.\n", (long)threadID);
		ProgrammingTime = rand() % 10 + 1;
		sleep(ProgrammingTime);		
		printf("Student %ld needs help from the TA\n", (long)threadID);
		pthread_mutex_lock(&CAccess);
		int count = CCount;
		pthread_mutex_unlock(&CAccess);
		if(count < 3)		
		{
			If          (count == 0)		
				sem_post(&TA_Sleep);
			else
printf("Student %ld sat on a chair waiting for the TA to finish. \n",   (long)threadID);					
			pthread_mutex_lock(&CAccess);
			int index = (CurrentIndex + CCount) % 3;
			CCount++;
			printf("Student sat on chair.Chairs Remaining: %d\n", 3 - CCount);
			pthread_mutex_unlock(&CAccess);
			sem_wait(&CSem[index]);		
			printf("\t Student %ld is getting help from the TA. \n", (long)threadID);
			sem_wait(&SSem);		
			printf("Student %ld left TA room.\n",(long)threadID);
		}
		else 
			printf("Student %ld will return at another time. \n", (long)threadID);   }  }
			

