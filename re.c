/* re is a regular expression engine written in C.
 *
 * See: http://swtch.com/~rsc/regexp/regexp1.html
 */


#include "nfa.h"


State* post2nfa(char *postfix)
{
	char *p;
	Frag stack[1000], *stackp, e1, e2, e;
	State *s;

    /* 
     * C arrays are really a contiguous block of memory that the language
     * interprets as all of the same type. These helper functions just move the
     * array's pointer.
     */
	#define push(s) *stackp++ = s
	#define pop()   *--stackp

	stackp = stack;
	return e.start;
}

int main(int argc, char **argv)
{
    return 0;
}
