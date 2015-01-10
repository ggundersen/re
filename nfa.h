#ifndef NFA_H
#define NFA_H


/* We represent the NFA as a linked collection of State structs. */
struct State
{
    int c;
    State *out;
    State *out1;
    int lastlist;
};

/* An NFA fragment, containing a start state and 0 or more pointers to out
 * states.
 */
struct Frag
{
	State *start;
	Ptrlist *out;
}i;


#endif
