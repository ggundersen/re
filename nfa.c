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
 * An NFA fragment contains a start state and 0 or more pointers to out
 * states.
 */
struct Frag
{
	State *start;
	//Ptrlist *out;
	State *out_ptrs[2];
};

/* Creates a new pointer list containing a single pointer outp. */
State **new_ptrlist(State **outp)
{
    State **nlist = outp;
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
