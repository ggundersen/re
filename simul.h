#ifndef SIMUL_H
#define SIMUL_H


#include <stdio.h>
#include <string.h>
#include "nfa.h"


typedef struct StateList StateList;
struct StateList
{
    /* 
     * We don't have to use a linked list of States because we know exactly how
     * big the list should be at construction. It is the number of NFA states
     * created for the NFA multipled by the size of the State struct.
     */
	State **s;
	/* Tracks the number of states on the StateList struct. */
	int n;
};

int match(State *start, char *s);


#endif
