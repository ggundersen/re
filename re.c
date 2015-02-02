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
    Frag stack[1000], e1, e2;

    stackp = stack;
    for (p = postfix; *p != '\0'; p++) {
        /*
         * This switch statement builds the NFA with a stack. Because the input
         * expression is in postfix notation, we never go more than a single
         * char without seeing a metacharacter.
         */
		switch (*p) {
		    /* Character literal. */
            default:
            	s = State_new(*p, NULL, NULL);
	            /* 
	             * s->out1 is a pointer; therefore &(s->out1) is a pointer to a
	             * pointer.
	             */
	            out_ptrs = OutPtrs_new(&(s->out1));
	            f = Frag_new(s, out_ptrs);
            	push(f);
	            break;

            /* Explicit concatenation. */
            case '.':
                e2 = pop();
                e1 = pop();
                patch(e1.outPtrs, e2.start);
                f = Frag_new(e1.start, e2.outPtrs);
                push(f);
                break;

            /* Alternation. */
            case '|':
            	e2 = pop();
            	e1 = pop();
            	/* TODO: Use the Split enum. */
            	s = State_new('~', e1.start, e2.start);
            	out_ptrs = concat(e1.outPtrs, e2.outPtrs);
            	f = Frag_new(s, out_ptrs);
            	push(f);
            	break;

            /* Kleene star: zero or more. */
		    case '*':
		        e1 = pop();
			    s = State_new('~', e1.start, NULL);
			    patch(e1.outPtrs, s);
	            out_ptrs = OutPtrs_new(&(s->out2));
	            f = Frag_new(s, out_ptrs);
			    push(f);
			    break;
			}
	}

	e1 = pop();
	patch(e1.outPtrs, &match_state);
    return e1.start;
}

int main(int argc, char **argv)
{
    char *input = "aaaaaaaaaaaa";
    State *start = post2nfa("a*");
    if (match(start, input)) {
        printf("%s matches\n", input); 
    }
    return 0;
}
