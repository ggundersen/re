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
	/* These pointers now share a pointee. */
	s->out1 = out1;
	s->out2 = out2;
	return s;
}

/* TODO: Use enum. */
State match_state = { '!' };

/* Creates a new pointer list containing a single pointer outp. */
OutPtrs *OutPtrs_new(State **outpp)
{
    OutPtrs *slist = malloc(sizeof *slist);
    /* 
     * outpp is a pointer to a pointer to a State. Dereference it once to get a
     * pointer to a State.
     */
    slist->s = outpp;
    slist->next = NULL;
    return slist;
}

Frag Frag_new(State *start, OutPtrs *outPtrs)
{
    /* 
     * We don't use a pointer to a Frag because we want Frags to be passed by
     * value rather than by reference. This allows us to reuse the State
     * pointers in the main switch statement: when a new Frag is created, new
     * State structs are created.
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
		*slist->s = end;
	}
}
