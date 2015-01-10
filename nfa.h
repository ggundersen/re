#ifndef NFA_H
#define NFA_H


/* These typedefs allows us to reference `X` rather than `Struct X`. */

typedef struct State State;
/* I'm not sure why RSC uses a union: http://stackoverflow.com/a/346579/1830334. */
typedef union Ptrlist Ptrlist;
typedef struct Frag Frag;



#endif
