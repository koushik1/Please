#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

struct  lentry  locks[NLOCKS];

void linit()
{

    struct lentry *lptr;
    int i=0;
    nextlock = NLOCKS - 1;

    while(i < NLOCKS){

        lptr = &locks[i];
        lptr->lstate = LFREE;
        lptr->lqhead = newqueue();
        lptr->lqtail = 1 + lptr->lqhead;
	    lptr->lprio  = -1;	

        int j=0;
        while(j<NPROC)
        {
            lptr->process_map[j] = 0;
            j++;
        }
        i++;
    }

}
