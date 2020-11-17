#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>


SYSCALL ldelete(int lockdescriptor)
{
	STATWORD ps;    
	int	pid;
	struct	lentry	*lptr;
	int i;

	disable(ps);
	if ((lockdescriptor<0 || lockdescriptor>=NLOCKS) || locks[lockdescriptor].lstate==LFREE) {
		restore(ps);
		return(SYSERR);
	}
	lptr = &locks[lockdescriptor];
	lptr->lstate = LFREE;
	lptr->ltype = DELETED;
	lptr->lprio = -1;

	for (i=0;i<NPROC;i++)
	{
		if (lptr->process_map[i] == 1)
		{
			lptr->process_map[i] = 0;
			proctab[i].bm_locks[lockdescriptor] = 0;
		}
	}	
	
	if (nonempty(lptr->lqhead)) {
		while( (pid=getfirst(lptr->lqhead)) != EMPTY)
		  {
		    proctab[pid].plockret = DELETED;
		    proctab[pid].wait_lockid = -1;	
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
	return(OK);
}
