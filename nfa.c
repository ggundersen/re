#include "nfa.h"


/* We represent the NFA as a linked collection of State structs. */
struct State
{
    int c;
    State *out;
    State *out1;
    int lastlist;
};

/* I'm not sure why RSC uses a union: http://stackoverflow.com/a/346579/1830334. */
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
	struct State *start;
	Ptrlist *out;
};
