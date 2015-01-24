#include "simul.h"


/* This global keeps track of which NFA simulation we are on. */
int g_list_id = 0;

void add_state(List *l, State *s)
{
    /* 
     * The "s == NULL" guard checks that we are not trying to add a dangling
     * pointer. The "s->list_id == g_list_id" guard checks that we haven't
     * already added the State to the list being built.
     */
	if (s == NULL || s->list_id == g_list_id)
		return;
	s->list_id = g_list_id;
	/* TODO: ~ is my hack to denote split. Change this to a real enum. */
	if (s->c == '~') {
		add_state(l, s->out1);
		add_state(l, s->out2);
		return;
	}
	l->s[l->n++] = s;
}

/* 
 * Notice this does *not* create a new List struct. The memory has already been
 * allocated. This adds a start State and any subsequent States to the List l.
 */
List* init_list(State *s, List *l)
{
	g_list_id++;
	/* n tracks the number of State structs on the list. */
	l->n = 0;
	add_state(l, s);
	return l;
}

int is_match(List *l)
{
	int i;
	for (i = 0; i < l->n; i++)
	    /* TODO: Use the enum with Match == 256. */
	    if (l->s[i]->c == '-')
	        return 1;
	return 0;
}

void step(List *clist, int c, List *nlist)
{
	int i;
	State *s;
	g_list_id++;

	/* 
	 * Every step, we reuse clist and nlist to capture the current NFA states.
	 * Reset n every step 
	 */
	nlist->n = 0;
	for (i = 0; i < clist->n; i++) {
		s = clist->s[i];
		if (s->c == c) {
			add_state(nlist, s->out1);
		}
	}
}

int match(State *start, char *s, List *curr_list, List *next_list)
{
	List *clist, *nlist, *temp;

	clist = init_list(start, curr_list);
	nlist = next_list;
	for (; *s; s++) {
		step(clist, *s, nlist);
		/* Swap. */
		temp = clist;
		clist = nlist;
		nlist = temp;
	}
	return is_match(clist);
}
