/* re is a regular expression engine written in C.
 *
 * See: http://swtch.com/~rsc/regexp/regexp1.html
 */


#include <stdio.h>
#include "nfa.h"


Frag *stackp;

Frag pop()
{
    return *--stackp;
}

void push(Frag s)
{
    stackp++;
    stackp = &s;
}

State* post2nfa(char *postfix)
{
	char *p;
	Frag stack[1000], /* stackp, */ e1, e2/*, e*/;
	State *s;

    /* 
     * C arrays are really a contiguous block of memory that the machine
     * interprets as all of the same type. These helper functions just move the
     * array's pointer.
     */
	//#define push(s) *stackp++ = s
	//#define pop() *--stackp

    /*
     * In C, strings can be iterated over by incrementing a pointer to the
     * first character.
     */
    stackp = stack;
    Frag f;
    for (p = postfix; *p; p++) {
		switch (*p) {
		    default:
		        //printf("%c\n", *p);
	            s = State_new(*p, NULL, NULL);
	            f = Frag_new(s, Unconn_ptr_list_new(s->out1));
	            //printf("%c\n", *p);
	            push(f);
	            break;
            case '|':
            	//printf("%c\n", *p);
            	e2 = pop();
            	e1 = pop();
            	/* TODO: Use the Split enum. */
            	s = State_new(257, e1.start, e2.start);
            	f = Frag_new(s, concat(e1.out, e2.out));
            	push(f);
            	break;
        }
	}
	
	return s;
}

int main(int argc, char **argv)
{
    post2nfa(argv[1]);
    return 0;
}
