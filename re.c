/* re is a regular expression engine written in C.
 *
 * See: http://swtch.com/~rsc/regexp/regexp1.html
 */


#include <stdio.h>
#include "nfa.h"
#include "simul.h"

/* Silences the unused variable warning. */
#define UNUSED(x) (void)(x)


/* 
 * Tracks the number of States in the NFA so that we can allocate enough memory
 * to simulate the NFA.
 */
int num_states;
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
 * cannot handle an infix expression, for example "a|b".
 */
State *post2nfa(char *postfix)
{
    /* 
     * A pointer is a variable whose value is the address of another variable;
     * importantly, the compiler knows how to interpret the data at that
     * address because of the pointer's type.
     *
     * Reusing a pointer in a loop, which is what we are about to do here, is
     * fine; For example, *s can be reused over and over; the value pointed to
     * by s is a State struct, but that struct is reassigned to *newly
     * allocated memory* in the State_new() constructor.
     *
     * Appreciate when memory is being reused; when it is in read-only memory,
     * i.e. on the stack; and when it is read- and writeable, i.e. on the heap.
     * Read this for details: http://stackoverflow.com/a/80113/1830334.
     */
	char *p;
	State *s;
    Frag f;
    OutPtrs *out_ptrs;

    /*
     * In C, strings can be iterated over by incrementing a pointer to the
     * first character.
     */   
    Frag stack[1000], e1, e2, e;
    stackp = stack;

    for (p = postfix; *p != '\0'; p++) {
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
            default:
	            s = State_new(*p, NULL, NULL);
	            /* 
	             * Pass the *address* to out1. If we passed the *value*, we
	             * would be passing NULL.
	             */
	            out_ptrs = OutPtrs_new(&s->out1);
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
        num_states++;
	}
	
	e = pop();
	patch(e.outPtrs, &match_state);
	return e.start;
}

int main(int argc, char **argv)
{
    num_states = 0;
    State *start = post2nfa("ab|");
    //UNUSED(start);

    /* 
     * Allocate enough memory for two lists to keep track of the current states
     * of the simulated NFA.
     */
    /*List curr_states;
    curr_states.s = malloc(num_states * sizeof(State) * 1000);
    List next_states;
    next_states.s = malloc(num_states * sizeof(State) * 1000);
    if (match(start, "a", &curr_states, &next_states)) {
        printf("%s matches\n", "a"); 
    } else
        printf("Fail\n");*/
    return 0;
}
