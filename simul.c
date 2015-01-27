#include "simul.h"


/* This global keeps track of which NFA simulation we are on. */
int g_list_id = 0;

void add_state(StateList *l, State *s)
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

void step(StateList *clist, int c, StateList *nlist)
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

/* 
 * Notice this does *not* create a new StateList struct. The memory has already
 * been allocated. This adds a start State and any subsequent States to the
 * StateList l.
 */
StateList* init_list(State *s, StateList *l)
{
	g_list_id++;
	/* n tracks the number of State structs on the list. */
	l->n = 0;
	add_state(l, s);
	return l;
}

StateList *StateList_new()
{
    /* TODO: 1000 should be the number of NFA Frags created in post2nfa(). */
    int states_size = 1000 * sizeof(State);
    int int_size = sizeof(int);

    StateList *l = malloc(int_size + states_size);
    l->s = malloc(states_size);
    l->n = 0;
    return l;
}

int is_match(StateList *l)
{
	int i;
	for (i = 0; i < l->n; i++)
	    /* TODO: Use the enum with Match == 256. */
	    if (l->s[i]->c == '-')
	        return 1;
	return 0;
}

int match(State *start, char *s)
{
    /* 
     * Allocate enough memory for two lists to keep track of the current states
     * of the simulated NFA.
     */
	StateList *clist, *nlist, *temp;
	clist = init_list(start, StateList_new());
	nlist = StateList_new();
	for (; *s; s++) {
		step(clist, *s, nlist);
		/* Swap. */
		temp = clist;
		clist = nlist;
		nlist = temp;
	}
	return is_match(clist);
}
