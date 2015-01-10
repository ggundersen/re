#include "nfa.h"


/* Use 256 and 257 for Match and Split to stay outside of ASCII's range. */
enum {
	Match = 256,
	Split = 257
};

/* We represent the NFA as a linked collection of State structs. */
struct State
{
    int c;
    State *out1;
    State *out2;
    int lastlist;
};

/* 
 * I'm not sure why RSC uses a union. See:
 * http://stackoverflow.com/a/346579/1830334.
 */
union Ptrlist
{
    Ptrlist *next;
    State *s;
};

/* 
 * An NFA fragment contains a start state and 0 or more pointers to out
 * states.
 */
struct Frag
{
	State *start;
	//Ptrlist *out;
	State *dangling_out_ptrs[2];
};


/* Creates a new pointer list containing a single pointer outp. */
Ptrlist *new_ptrlist(State **outp)
{
    /*
     * Called "type punning": http://en.wikipedia.org/wiki/Type_punning.
     * "This works because C does not care about the underlying data in an
     * address, just what you represent it as."
     */
    Ptrlist *nlist = (Ptrlist*) outp;
    nlist->next = NULL;
    return nlist;
}

/* Concatenates two pointer lists. */
Ptrlist *append(Ptrlist *l1, Ptrlist *l2)
{
    l1->next = l2;
    return l1;
}

/* Connects the dangling pointers in pointer list l to state s. */
void patch(Ptrlist *l, State *s)
{
    while (l->next)
        l->s = s;
}
