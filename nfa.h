#ifndef NFA_H
#define NFA_H


#include <stdlib.h>


/* Use 256 and 257 for Match and Split to stay outside of ASCII's range. */
enum test {
	Match = 256,
	Split = 257
};

/* These typedefs allows us to reference "X" rather than "Struct X". */

/* We represent the NFA as a linked collection of State structs. */
typedef struct State State;
struct State
{
    int c;
    State *out1;
    State *out2;
    int lastlist;
};

State *State_new(int c, State *out1, State *out2);

/* 
 * An NFA fragment contains a start state and 0 or more pointers to out
 * states.
 */
typedef struct Frag Frag;
struct Frag
{
	State *start;
	//Ptrlist *out;
	State **out;
};

/* Frag_new() does not return a pointer because Frags are stored on stack[]. */
Frag Frag_new(State *start, State **out);

/* Helper functions for manipulating NFA states. */
State **List_new(State *outp);


State **concat(State **l1, State **l2);
//void patch(Ptrlist *l, State *s);


#endif
