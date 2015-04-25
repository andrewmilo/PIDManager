// BY ANDREW MILOSLAVSKY
// OPERATING SYSTEMS 340
// MARCH 26 2015

#ifndef PIDMANAGER
#define PIDMANAGER

#include <stddef.h>
#include <pthread.h>

/*This structure represents a segment of 
contiguous memory with 'numberOfSlots' divisions*/
typedef struct MemorySpace_t
{
	int *head;
	int numberOfSlots;
	pthread_mutex_t mutexsum;
} MemorySpace;

//Returns an available PID
int AllocatePID(MemorySpace);

//Releases a PID from use
void ReleasePID(MemorySpace, int);

#endif
