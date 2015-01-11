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
	            printf("%d\n", s->out1 == NULL);
	            f = Frag_new(s, List_new(s->out1));
	            printf("%c\n", f.start->c);
	            push(f);
	            break;
            case '|':
            	e2 = pop();
            	e1 = pop();
            	s = State_new(Split, e1.start, e2.start);
            	f = Frag_new(s, concat(e1.out, e2.out));
            	push(f);
            	break;
        }
	}
	
	//e = pop();
	//patch(e.out, matchstate);
	//return e.start;
	return s;
}

int main(int argc, char **argv)
{
    post2nfa(argv[1]);
    return 0;
}
