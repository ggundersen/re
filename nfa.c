#include "nfa.h"
#include <stdio.h>

/*
 * A helpful constructor function for the State struct. See:
 * http://stackoverflow.com/a/3774202/1830334.
 */
State *State_new(char c, State *out1, State *out2)
{
	/* 
	 * This is used to correctly allocate enough memory for the NFA. In RSC's
	 * implementation, it is a global.
	 */
	//state_count++;

	State *s;
	s = malloc(sizeof *s);
	s->lastlist = 0;
	s->c = c;
	s->out1 = out1;
	s->out2 = out2;
	return s;
}

/* See State_new for details about this pattern. */
Frag Frag_new(State *start, State **ptrs)
{
	Frag n = { start, ptrs };
	return n;
}

/* Creates a new pointer list containing a single pointer outp. */
State **List_new(State *out_ptr)
{
    /* Take the address of out_ptr and set it as the first element. */
    State **nlist = &out_ptr;
    return nlist;
}

/* Concatenates two pointer lists. */
/*State **append(State **l1, State **l2)
{
    l1[0] = l2;
    return l1;
}*/

/* Connects the dangling pointers in pointer list l to state s. */
/*void patch(Ptrlist *l, State *s)
{
    while (l->next)
        l->s = s;
}*/
