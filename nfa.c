#include <stdio.h>
#include "nfa.h"
#include "simul.h"


/*
 * Constructor-style function for States a la
 * http://stackoverflow.com/a/3774202/1830334.
 */
State *State_new(char c, State *out1, State *out2)
{
	State *s;
	s = malloc(sizeof(*s));
	s->list_id = 0;
	s->c = c;
	s->out1 = out1;
	s->out2 = out2;
	return s;
}

/* TODO: Use enum. */
State match_state = { '!' };

/* Creates a new pointer list containing a single pointer outp. */
OutPtrs *OutPtrs_new(State *outp)
{
    OutPtrs *slist = malloc(sizeof *slist);
    /* dereference outp once to get a pointer to a State. */
    slist->s = outp;
    slist->next = NULL;
    return slist;
}

/* 
 * We don't create a pointer to a Frag, like we do with State and OutPtrs,
 * because objects are all referentiable from within post2nfa().
 */
Frag Frag_new(State *start, OutPtrs* outPtrs)
{
    /* 
     * Why doesn't this work for *State_new and *OutPtrs_new? Because we want
     * to get a pointer back from those functions. malloc() allocates memory
     * and returns and address for that memory.
     *
     * Why is it okay that this returns a copy of the newly instantiated Frag?
     */
	Frag n = { start, outPtrs };
	return n;
}

/* Concatenates two pointer lists. */
OutPtrs *concat(OutPtrs *l1, OutPtrs *l2)
{
    /* Save the start pointer and then traverse to the end of the linked list. */
    OutPtrs *start = l1;
    while (l1->next)
        l1 = l1->next;
    /* l1->next is NULL; it is safe to assign a new OutPtrs to it. */
    l1->next = l2;
    
    /* 
     * This is an example of pointer power/confusion. start is a pointer to
     * the start of OutPtrs. But it has been concatenated with l2 because we
     * set l1->next to l2.
     */
    return start;
}

/* 
 * Connects the unconnected pointers in the linked list of States structs to
 * new out state.
 */
void patch(OutPtrs *slist, State *end)
{
    OutPtrs *next;

	for (; slist; slist = next) {
		next = slist->next;
		slist->s = end;
	}
}
