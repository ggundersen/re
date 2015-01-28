#include <stdio.h>
#include "nfa.h"
#include "simul.h"


/*
 * A helpful constructor function for the State struct. See:
 * http://stackoverflow.com/a/3774202/1830334.
 */
State *State_new(char c, State *out1, State *out2)
{
	State *s;
	s = malloc(sizeof *s);
	s->list_id = 0;
	s->c = c;
	s->out1 = out1;
	s->out2 = out2;
	return s;
}

/* TODO: Use enum. */
State match_state = { '-' };

/* Creates a new pointer list containing a single pointer outp. */
OutPtrs *OutPtrs_new(State **outp)
{
    OutPtrs *slist = malloc(sizeof *slist);
    /* 
     * outp is a pointer to a pointer of a State struct. Thus, these two
     * notations refer to the same pointer:
     *     outp
     *     (State*) outp
     *
     * We choose the latter notation because we cannot assign outp to
     * slist->s directly because the compiler does not know they are of the
     * same type.
     */
    slist->s = (State*) outp;
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
     * We can instantiate a struct using C's designated initializer. See
     * http://stackoverflow.com/a/330867/1830334 for more details.
     *
     * Why doesn't this work for *State_new and *OutPtrs_new? Because we want
     * to get a pointer back from those functions. malloc() allocates memory
     * and returns and address for that memory.
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
    printf("patch:\n");

	for (; slist; slist = next) {
	    printf("%c\n", end->c);
		next = slist->next;
		printf("%p\n", &(slist->s));
		slist->s = end;
	}
}
