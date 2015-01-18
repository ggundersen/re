/* re is a regular expression engine written in C.
 *
 * See: http://swtch.com/~rsc/regexp/regexp1.html
 */


#include <stdio.h>
#include "nfa.h"
#include "simul.h"


Frag *stackp;
    
/* 
 * C arrays are really a contiguous block of memory that the machine interprets
 * as all of the same type. pop() and push() handle moving the stack pointer so
 * we can dereference the correct NFA fragment.
 */
Frag pop()
{
    /* 
     * The stack pointer moves forward every time push() is called. When we
     * pop(), we first decrement the stack pointer to find the previous State
     * and then dereference the pointer to get the stack.
     */
    stackp--;
    return *stackp;
}

void push(Frag f)
{
    /* 
     * Assign the fragment to stack[] by dereferencing the pointer and then
     * increment the pointer.
     */
    *stackp = f;
    stackp++;
}

/* 
 * Takes a postfix expression, e.g. "ab|" for "a" literal or "b" literal. It
 * cannot handle an infix expression, e.g. "a|b".
 */
State *post2nfa(char *postfix)
{
	char *p;
	State *s;
    Frag f;
    
    /*
     * In C, strings can be iterated over by incrementing a pointer to the
     * first character.
     */   
    Frag stack[1000], e1, e2, e;
    stackp = stack;
    
    for (p = postfix; *p; p++) {
        /*
         * This switch statement builds the NFA with a stack. When it
         * encounters an operator, it pops the last fragment(s) off of the
         * stack and patches it (them) to create a new fragment. When it
         * encounters a character literal, it simply pushes a new fragment onto
         * the stack.
         */
		switch (*p) {
		    /* 
		     * char literal. We don't know what to do with this fragment until
		     * we see the upcoming metacharacter. Because this is a postfixed
		     * string, we never go more than a single char without seeing a
		     * metacharacter.
		     */
            case '|':
                printf("%c\n", *p);
            	e2 = pop();
            	e1 = pop();
            	/* TODO: Use the Split enum. */
            	s = State_new(257, e1.start, e2.start);
            	f = Frag_new(s, concat(e1.out, e2.out));
            	push(f);
            	break;
            default:
		        printf("%c\n", *p);
	            s = State_new(*p, NULL, NULL);
	            State **ptrs = Unconn_ptrs_new(s->out1);
	            f = Frag_new(s, ptrs);
	            push(f);
	            break;
        }
	}
	
	e = pop();
	patch(e.out, &match_state);
	return s;
}

int main(int argc, char **argv)
{
    post2nfa(argv[1]);
    /*char *word = argv[2];
    State *start = post2nfa(argv[1]);
    if (match(start, word))
        printf("%s matches\n", word);*/
    return 0;
}
