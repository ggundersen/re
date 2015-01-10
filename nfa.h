#ifndef NFA_H
#define NFA_H


#define NULL 0


/* These typedefs allows us to reference "X" rather than "Struct X". */

typedef struct State State;
/* I'm not sure why RSC uses a union: http://stackoverflow.com/a/346579/1830334. */
typedef union Ptrlist Ptrlist;
typedef struct Frag Frag;

/* Helper functions for manipulating NFA states. */
State **new_ptrlist(State **outp);
//State **append(Ptrlist *l1, Ptrlist *l2);
//void patch(Ptrlist *l, State *s);


#endif
