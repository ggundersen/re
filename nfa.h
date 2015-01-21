#ifndef NFA_H
#define NFA_H


#include <string.h>
#include <stdlib.h>


/* TODO! */
/* Use 256 and 257 for Match and Split to stay outside of ASCII's range. */
/*enum {
	Match = 256,
	Split = 257
};*/

/* These typedefs allows us to reference "X" rather than "Struct X". */

/* We represent the NFA as a linked collection of State structs. */
typedef struct State State;
struct State
{
    int c;
    State *out1;
    State *out2;
    int list_id;
};

State *State_new(int c, State *out1, State *out2);

State match_state;

/*
 * A linked list of state structs.
 */
typedef struct StateList StateList;
struct StateList
{
    State *s;
    StateList *next;
};

/* 
 * An NFA fragment contains a start state and 0 or more pointers to out
 * states.
 */
typedef struct Frag Frag;
struct Frag
{
	State *start;
	StateList *out;
};

/* Frag_new() does not return a pointer because Frags are stored on stack[]. */
Frag Frag_new(State *start, StateList *out);

/* Helper functions for manipulating NFA states. */
StateList *StateList_new(State *outp);

StateList *concat(StateList *l1, StateList *l2);

//void patch(State **unconn_out_ptrs, State *s);


#endif
