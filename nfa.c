#include <stdio.h>
#include "nfa.h"
#include "simul.h"


/*
 * A helpful constructor function for the State struct. See:
 * http://stackoverflow.com/a/3774202/1830334.
 */
State *State_new(int c, State *out1, State *out2)
{
	State *s;
	s = malloc(sizeof *s);
	s->list_id = 0;
	s->c = c;
	s->out1 = out1;
	printf("Initializing in STATE:  %p\n", &s->out1);
	s->out2 = out2;
	return s;
}

/* TODO: Use enum. */
State match_state = { '-' };

/* Creates a new pointer list containing a single pointer out_ptr. */
OutPtrs *OutPtrs_new(State **out)
{
    OutPtrs *slist = malloc(sizeof *slist);
    /* Cast an pointer to a pointer of a State struct to a pointer of a State struct. This seems DANGEROUS. */
    slist->s = (State*)out;
    printf("Initializing in OUTPRS: %p\n", out);
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
     * http://stackoverflow.com/a/330867/1830334 for more details. Why doesn't
     * this work for *State_new and *OutPtrs_new? Because we want to get a
     * pointer back from those functions. malloc() allocates memory and returns
     * and address for that memory.
     */
	Frag n = { start, outPtrs };
	return n;
}

/* Concatenates two pointer lists. */
OutPtrs *concat(OutPtrs *l1, OutPtrs *l2)
{
    /* Save the original pointer before traversing the linked list. */
    OutPtrs *original = l1;

    /* 
     * Traverse the linked list.
     * NULL evaluates to false. This is guaranteed by ANSI C. See:
     * http://stackoverflow.com/a/459757/1830334
     */
    while (l1->next)
        l1 = l1->next;

    /* l1->next is NULL; it is safe to assign a new OutPtrs to it. */
    l1->next = l2;
    
    /* 
     * This is an example of pointer power/confusion. original is a pointer to
     * the original OutPtrs. But it has been concatenated with l2 because we
     * set l1->next to l2.
     */
    return original;
}

/* 
 * Connects the unconnected pointers in the linked list of States structs to
 * new out state.
 */
void patch(OutPtrs *slist, State *s)
{
    OutPtrs *next;

	/* 
	 * This syntax means there is no initialization; just execute the code
	 * until the condition, that slist is not NULL, is false.
	 */
	for (; slist; slist = next) {
		next = slist->next;
		printf("Patching %p with %p & value %c\n", &slist->s, &s, s->c); 
		slist->s = s;
	}
}
