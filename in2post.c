#include "in2post.h"


/* Silences the unused variable warning. */
#define UNUSED(x) (void)(x)


/* 
 * The static keyword restricts the scope of pop() and push() so we do not have
 * symbol conflicts with the other pop and push functions. See:
 * http://stackoverflow.com/a/572550/1830334.
 */
static char pop(char **stackp)
{
    (*stackp)--;
    return **stackp;
}

static void push(char **stackp, char c)
{
    **stackp = c;
    (*stackp)++;
}

/* Details here: http://csis.pace.edu/~wolf/CS122/infix-postfix.htm. */
char *in2post(char *infix)
{
    char *p, op_stack[1000], *stackp, buff[8000], *post;

    post = buff;
    stackp = op_stack;
    *stackp = '~';
    stackp++;
    for (p = infix; *p != '\0'; p++) {
		switch (*p) {
            default:
                *post = *p;
                post++;
                break;

            case '+':
                push(&stackp, *p);
                break;
        }
    }

    /* Pop all remaining operators off of the stack. */
    while (*stackp != '~') {
        char c = pop(&stackp);
        printf("%c\n", c);
        *post = c; 
        post++;
    }

    printf("Final: %s\n", buff);
    return post;
}
