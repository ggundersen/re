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
	if (s == NULL) {
	    printf("NULL\n");
	    return;
	}
	if (s->list_id == g_list_id) {
	    printf("%d", s->list_id);
	    return;
	}

    printf("adding %c\n", s->c);

	s->list_id = g_list_id;
	/* TODO: ~ is my hack to denote split. Change this to a real enum. */
	if (s->c == '~') {
	    printf("adding in split %c\n", s->out1->c);
		add_state(l, s->out1);
		printf("adding in split %c\n", s->out2->c);
		add_state(l, s->out2);
		return;
	}
	l->states[l->length++] = s;
}

void step(int c, StateList *clist, StateList *nlist)
{
	int i;
	State *s;
	g_list_id++;

	/* Reset the length because we're going to use this list. */
	nlist->length = 0;

	for (i = 0; i < clist->length; i++) {
		s = clist->states[i];
		//printf("%c\n", s->c);
		if (s->c == c) {
		    printf("%c\n", s->c);
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
	l->length = 0;
	add_state(l, s);
	return l;
}

StateList *StateList_new()
{
    /* TODO: 1000 should be the number of NFA Frags created in post2nfa(). */
    int states_size = 1000 * sizeof(State);
    int int_size = sizeof(int);

    StateList *l = malloc(int_size + states_size);
    l->states = malloc(states_size);
    l->length = 0;
    return l;
}

int is_match(StateList *l)
{
	int i;
	printf("%d\n", l->length);
	for (i = 0; i < l->length; i++) {
	    printf("looking for match %c\n", l->states[i]->c);
	    /* TODO: Use the enum with Match == 256. */
	    if (l->states[i]->c == '-')
	        return 1;
	}
	return 0;
}

int match(State *start, char *s)
{
	StateList *clist, *nlist, *temp;
    /* 
     * Allocate enough memory for two lists to keep track of the current states
     * of the simulated NFA.
     */
	clist = init_list(start, StateList_new());
	printf("done building frag list\n");
	nlist = StateList_new();

	for (; *s; s++) {
	    printf("next character in input word is %c\n", *s);
		step(*s, clist, nlist);
		/* Swap. */
		printf("swapping\n");
		temp = clist;
		clist = nlist;
		nlist = temp;
	}
	printf("looking for a match\n");
	return is_match(nlist);
}
