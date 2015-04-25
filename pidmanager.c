// BY ANDREW MILOSLAVSKY
// OPERATING SYSTEMS 340
// MARCH 26 2015

//Compile with -pthread flag

#include <stdlib.h>      
#include <stdio.h>
#include "pidmanager.h"

int AllocatePID(MemorySpace memSpace)
{
	int i;
	for(i = 0; i < memSpace.numberOfSlots; i++)
	{
		//If slot is uninitialized
		if(memSpace.head[i] != 1)
		{
			//Set dirty
			memSpace.head[i] = 1;
			
			//Return PID
			return i;
		}
	}
	
	return -1;
}

void ReleasePID(MemorySpace memSpace, int PID)
{
	if(PID < memSpace.numberOfSlots)
	{
		//Set Lock
		pthread_mutex_lock(&memSpace.mutexsum);
		//Release
		memSpace.head[PID] = 0;
		//Unlock
		pthread_mutex_unlock (&memSpace.mutexsum);
	}
}
