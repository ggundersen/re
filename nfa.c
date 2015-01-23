#include <stdio.h>
#include "nfa.h"
#include "simul.h"


/*
 * A helpful constructor function for the State struct. See:
 * http://stackoverflow.com/a/3774202/1830334.
 */
State *State_new(int c, State *out1, State *out2)
{
	/* 
	 * This is used to correctly allocate enough memory for the NFA. In RSC's
	 * implementation, it is a global.
	 */
	//state_count++;

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

/* Creates a new pointer list containing a single pointer out_ptr. */
StateList *StateList_new(State *out)
{
    StateList *slist;
    slist = malloc(sizeof *slist);
    slist->s = out;
    slist->next = NULL;
    return slist;
}

/* 
 * We don't create a pointer to a Frag, like we do with State and StateList,
 * because objects are all referentiable from within post2nfa().
 */
Frag Frag_new(State *start, StateList* outList)
{
    /* 
     * We can instantiate a struct using C's designated initialier. See
     * http://stackoverflow.com/a/330867/1830334 for more details. Why doesn't
     * this work for *State_new and *StateList_new? Because we want to get a
     * pointer back from those functions. malloc() allocates memory and returns
     * and address for that memory.
     */
	Frag n = { start, outList };
	return n;
}

/* Concatenates two pointer lists. */
StateList *concat(StateList *l1, StateList *l2)
{
    /* Save the original pointer before traversing the linked list. */
    StateList *original = l1;

    /* 
     * Traverse the linked list.
     * NULL evaluates to false. This is guaranteed by ANSI C. See:
     * http://stackoverflow.com/a/459757/1830334
     */
    while (l1->next) {
        l1 = l1->next;
    }

    /* l1->next is NULL; it is safe to assign a new StateList to it. */
    l1->next = l2;
    
    /* 
     * This is an example of pointer power/confusion. original is a pointer to
     * the original StateList. But it has been concatenated with l2 because we
     * set l1->next to l2.
     */
    return original;
}

/* Connects the unconnected pointers to new out state. */
void patch(StateList *slist, State *s)
{
    /* 
     * next is a good example of something I wouldn't think to do in another
     * language; or perhaps I simply don't know what is happening in another
     * language: we are reusing the pointer to a StateList to traverse the 
     * linked list. On every iteration of the while loop, we reuse that block
     * of memory--which the computer can correctly interpret because it is
     * always a StateList.
     */
    StateList *next;

    slist->s = s;
    while (slist) {
        next = slist->next;
        slist->s = s;
        /* Traverse the linked list; next is incremented every loop. */
        slist = next;
    }
}
