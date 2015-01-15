#ifndef SIMUL_H
#define SIMUL_H


#include <stdio.h>
#include "nfa.h"


typedef struct List List;
struct List
{
	State **s;
	/* n tracks the number of States in the list. */
	int n;
};

int match(State *start, char *s);


#endif
