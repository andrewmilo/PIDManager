// BY ANDREW MILOSLAVSKY
// OPERATING SYSTEMS 340
// MARCH 26 2015
// Run Parameters (1): # of threads

//GCC Compile -> gcc -o threadtest threadtest.c pidmanager.c -pthread

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "pidmanager.h"
#include <string.h>

void OnThreadCreation(int);
void DestroyThread(int);

//Shared Memory Space
MemorySpace memSpace;

int main(argc, argv)
	int argc;
	char *argv[];
{
	if(argc < 3)
	{
		printf("%s", "Not enough parameters...Run parameters (2): # of threads to create, # number of slots");
		exit(0);
	}
	
	printf("\n");

	int numberOfThreads = atoi(argv[1]);
	int numberOfSlots = atoi(argv[2]);

	//Create an int array that will be shared amongst threads
	int SHM [numberOfSlots];

	//Clear new array
	memset(SHM, 0, sizeof(SHM));
	
	//Put array into structure of global scope
	memSpace.head = SHM;	
	memSpace.numberOfSlots = numberOfSlots;
	
	//Array of threads
	pthread_t allThreads[numberOfThreads];

	//Initialize the mutex
	pthread_mutex_init(&memSpace.mutexsum, NULL);
	
	//Create 'numberOfThreads' threads
	int i = 0;
	for(i = 0; i < numberOfThreads; i++)
	{
		int creationStatus = pthread_create(&allThreads[i], NULL, (void *)OnThreadCreation, (void *)i);
		
		if(creationStatus)
		{
			printf("Thread creation FAILED");
			exit(-1);
		}
	}
		
	//When finished, join all the created threads
	for(i = 0; i < numberOfThreads; i++)
		pthread_join(allThreads[i], NULL);
	
	//Destroy mutex and end threads
	pthread_mutex_destroy(&memSpace.mutexsum);
	pthread_exit(NULL);

	return 0;
}

void OnThreadCreation(int id)
{
	printf("Thread %d has been created!\n", id);	

	//Allocate a PID using the memSpace structure
	int allocatedPID = AllocatePID(memSpace);
	
	if(allocatedPID == -1)
		printf("**Thread %d could not find an available PID. It will loop until it claims a PID.**\n", id);

	//Loop until a slot is available
	while(allocatedPID == -1)
		allocatedPID = AllocatePID(memSpace);
		
	printf("**PID %d has been claimed by THREAD %d!**\n", allocatedPID, id);
	
	//Sleep from 0-5 seconds
	sleep(rand() % 6);	
	
	time_t t;
	srand((unsigned) time(&t));
	
	printf("**PID %d has been released by THREAD %d!**\n", allocatedPID, id);
	//Release PID
	DestroyThread(allocatedPID);
}

void DestroyThread(int PID)
{
	ReleasePID(memSpace, PID);
	pthread_exit((void*) 0);
}
