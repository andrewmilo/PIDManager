// BY ANDREW MILOSLAVSKY
// OPERATING SYSTEMS 340
// MARCH 26 2015

/* compile with -lm for rand*/

//GCC Compile -> gcc -o pidchild pidchild.c pidmanager.c

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/signal.h>        
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pidmanager.h"

int AccessSharedMemory(void);

int numberOfSlots;
int *pint;
int shmkey;
char *addr;
int walkno, start, shmid, matsize;

int main(argc, argv)
int argc;
char *argv[];
{
	/*Get Params*/
	walkno = atoi(argv[1]);
	start = atoi(argv[2]);
	matsize = atoi(argv[3]);
	shmkey = atoi(argv[4]);
	numberOfSlots = atoi(argv[5]);

	/*Create Shared Memory*/
	if(AccessSharedMemory() == 1)
	{
		MemorySpace memSpace = { (int *)addr, numberOfSlots};
						
		/*Allocate PID*/
		int allocatedPid = AllocatePID(memSpace);
		
		if(allocatedPid == -1)
			printf("**Child %d could not find an available PID. It will loop until it claims a PID.**\n", walkno);

		//Loop until a slot is available
		while(allocatedPid == -1)
			allocatedPid = AllocatePID(memSpace);
		
		printf ("\n** PID %d has been claimed by CHILD %d!**\n", allocatedPid, walkno);
		
		/*Sleep from 0-6 seconds*/
		sleep(rand() % 7);

		/*Release PID*/
		printf("\nPID %d has been released by CHILD %d!\n", allocatedPid, walkno);
		ReleasePID(memSpace, allocatedPid);
	}
	else
		printf("%s","Shared Memory not allocated! Exiting");

	/*Debug*/
	printf ("-> Child %d exiting", walkno);	
	
	return(0);
}

int AccessSharedMemory(void)
{
	/*set up shared memory*/
	shmid = shmget(shmkey, numberOfSlots * sizeof(int), 0777);
	addr = shmat(shmid, 0, 0);

	if(addr == NULL)
		return -1;

	return 1;
}
