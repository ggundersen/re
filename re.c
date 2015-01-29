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
	char *p;
	State *s;
    Frag f;
    OutPtrs *out_ptrs;
    Frag stack[1000], e1, e2, e;

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
	             * Pass the address instead of the value so we can change the
	             * value later.
	             */
	            out_ptrs = OutPtrs_new(s->out1);
                out_ptrs->s = State_new('c', NULL, NULL);
                printf("op: %c\n", s->out1->c);
	            f = Frag_new(s, out_ptrs);
            	push(f);
	            break;
            case '|':
            	e2 = pop();
            	e1 = pop();
            	/* TODO: Use the Split enum. */
            	s = State_new('~', e1.start, e2.start);
            	out_ptrs = concat(e1.outPtrs, e2.outPtrs);
            	f = Frag_new(s, out_ptrs);
            	push(f);
            	break;
        }
	}

	e = pop();
	patch(e.outPtrs, &match_state);

	return e.start;
}

int main(int argc, char **argv)
{
    State *start = post2nfa("a");
    UNUSED(start);

    /* 
     * Allocate enough memory for two lists to keep track of the current states
     * of the simulated NFA.
     */
    /*if (match(start, "a")) {
        printf("%s matches\n", "a"); 
    } else
        printf("Fail\n");*/
    return 0;
}
