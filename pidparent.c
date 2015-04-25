// BY ANDREW MILOSLAVSKY
// OPERATING SYSTEMS 340
// MARCH 26 2015

/* 
// Compile with arguments as such:
// basename, matsize, start, and # of slots
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define KEY 75

void AllocateSharedMemory(void);
void EndSharedMemory(void);

int numberOfSlots;
int shmid;
char *addr;

main(argc, argv)
	int argc;
	char *argv[];
{
	int matsize;
	int i,j,k,m,n, *pint, shmkey, offset, slotNumber;
	int status = 0;
	int walkers, walkpid, next,start;
	pid_t wpid;
	char thisstart[10];
	char thiswalkno[10];
	char thismatsize[10];
	char thisshmkey[10];
	char thisSlotNumber[10];

	/*Start processing with test of args*/
	if (argc < 4)
	{
		perror("Not enough parameters: basename, matsize, start, and # of slots");
		exit(0);
	}

	/*Get params*/
	matsize = atoi(argv[1]);
	start = atoi(argv[2]);
	numberOfSlots = atoi(argv[3]);

	/*Set up shared memory*/
	AllocateSharedMemory();

	/*Now create walkers*/
	for(m = 0; m < matsize; m++)
	{
		switch (walkpid = fork())
		{
			case -1:
			{
				perror("Bad fork");
				exit(0);
			}
			case 0:
			{
				sprintf(thiswalkno, "%d",m);
				sprintf(thisstart, "%d",start);
				sprintf(thismatsize, "%d",matsize);
				sprintf(thisshmkey, "%d",KEY);
				sprintf(thisSlotNumber, "%d",numberOfSlots); 
				
	 			execlp(
	 				"/home/os/linuxshare/pidchild", 
					"pidchild",
					thiswalkno,
					thisstart,
					thismatsize,
					thisshmkey,
					thisSlotNumber,
					NULL);
			}
			default: 
			{
				printf("Started walker %d\t%d\n", m, walkpid);
				
				if(m == matsize - 1)
					printf("\n");
			}
		}
	}

	//Sleep
	sleep(5);
	printf("\n\n*Unblocking*\n");
	
	/*wait for children to complete then terminate*/
 	while((wpid = wait(&status)) > 0);
        printf("All children completed\n");
        
     //Clean shared memory
     EndSharedMemory();
        
     printf("\n");
	printf("\n");
	printf("***%s***\n", "BY: ANDREW MILOSLAVSKY");
	printf("***%s***\n", "id: 14231872");
	printf("\n");
	printf("\n");
}

void AllocateSharedMemory(void)
{
	//Create shared memory
	shmid = shmget(KEY, numberOfSlots * sizeof(int), 0777| IPC_CREAT);
	//Attach shared memory
	addr = shmat(shmid,0,0);
	//Initialize shared memory contents
	memset(addr, 0, numberOfSlots * sizeof(int));
}

void EndSharedMemory(void)
{
	//Detach shm
     shmdt(addr);
	//Deallocate shm
	shmctl (shmid, IPC_RMID, 0);
}
