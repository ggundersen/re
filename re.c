/* re is a regular expression engine written in C.
 *
 * http://swtch.com/~rsc/regexp/regexp1.html
 * http://ezekiel.vancouver.wsu.edu/~cs317/archive/projects/grep/grep.pdf
 * http://csis.pace.edu/~wolf/CS122/infix-postfix.htm
 */


#include <stdio.h>
#include "nfa.h"
#include "simul.h"


/* Silences the unused variable warning. */
#define UNUSED(x) (void)(x)


// Verification
// 1. stack[] and stackp point to the correct Frags at the correct times.
// 2. State_new() constructor looks good.
// 3. Verified construction of Frags and Outptrs
// 4. Verified concat() and patch()
//
// Believe the bug is in simulation.


/* 
 * C arrays are really a contiguous block of memory that the machine interprets
 * as all of the same type. pop() and push() handle moving the stack pointer so
 * we can dereference the correct NFA fragment.
 */
Frag *stackp;

Frag pop()
{
    stackp--;
    return *stackp;
}

/* f is passed by value. Is this a problem? */
void push(Frag f)
{
    *stackp = f;
    stackp++;
}

/* 
 * Takes a postfix expression, e.g. "ab|" for "a" literal or "b" literal. It
 * cannot handle an infix expression, for example "a|b".
 */
State *post2nfa(char *postfix)
{
    /* 
     * When we assign a pointer x to a pointer y, both share the same pointee
     * or value. They do not share the same address. See:
     * http://cslibrary.stanford.edu/106/
     *
     * This means that when we create a new State pointer via State_new() and
     * assign it to *s, *s keeps its address but points to this new State
     * pointer.
     */
	State *s;
	char *p;
	/* 
	 * New Frags are created by value. This is why reusing *s works.
	 */
    Frag f;
    OutPtrs *out_ptrs;
    Frag stack[1000], e1, e2, e;

    State *t = State_new('t', NULL, NULL);

    stackp = stack;
    for (p = postfix; *p != '\0'; p++) {
        /*
         * This switch statement builds the NFA with a stack. Because the input
         * expression is in postfix notation, we never go more than a single
         * char without seeing a metacharacter.
         */
		switch (*p) {
            default:
            	s = State_new(*p, NULL, NULL);
	            /* 
	             * s->out1 is a pointer; therefore &(s->out1) is a pointer to a
	             * pointer.
	             */
	            out_ptrs = OutPtrs_new(&(s->out1));
	            f = Frag_new(s, out_ptrs);

                //printf("%c\n", t->c);
            	f.outPtrs->s = t;
            	s = t;
            	//printf("%p\n", &s);
            	printf("%c\n", s->c);
            	push(f);
	            break;
            case '|':
            	e2 = pop();
            	e1 = pop();
            	/* TODO: Use the Split enum. */
            	s = State_new('~', e1.start, e2.start);
            	out_ptrs = concat(e1.outPtrs, e2.outPtrs);
            	f = Frag_new(s, out_ptrs);
            	//printf("%p\n", &(f.outPtrs->s));
            	push(f);
            	break;
        }
	}
	//printf("-----");

	e = pop();
	patch(e.outPtrs, &match_state);

    return e.start;
}

int main(int argc, char **argv)
{
    State *start = post2nfa("ab|");
    UNUSED(start);

    /* 
     * Allocate enough memory for two lists to keep track of the current states
     * of the simulated NFA.
     */
    /*if (match(start, "a")) {
        printf("%s matches\n", "a"); 
    } else {
        printf("Fail\n");
    }*/
    return 0;
}
