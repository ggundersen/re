#include "simul.h"


/* Silences the unused variable warning. */
#define UNUSED(x) (void)(x)


/* This global keeps track of which NFA simulation we are on. */
int g_list_id = 0;


typedef struct StateList StateList;
struct StateList
{
    /* 
     * We don't have to use a linked list of States because we know exactly how
     * big the list should be at construction. It is the number of NFA states
     * created for the NFA multipled by the size of the State struct.
     *
     * We user a pointer instead of an array because we don't know how big the
     * array will be until runtime.
     */
	State **states;
	/* Tracks the number of states on the StateList struct. */
	int length;
};

void add_state(StateList *l, State *s)
{
    /* 
     * The "s == NULL" guard checks that we are not trying to add a dangling
     * pointer. The "s->list_id == g_list_id" guard checks that we haven't
     * already added the State to the list being built.
     */
	if (s == NULL) {
	    return;
	}
	if (s->list_id == g_list_id) {
	    return;
	}
	printf("%c\n", s->c);

	s->list_id = g_list_id;
	/* TODO: ~ is my hack to denote split. Change this to a real enum. */
	if (s->c == '~') {
		add_state(l, s->out1);
		add_state(l, s->out2);
		return;
	}
	l->states[l->length++] = s;
}

void step(char c, StateList *clist, StateList *nlist)
{
	int i;
	State *s;
	g_list_id++;

	/* Reset the length because we're going to use this list. */
	nlist->length = 0;

	for (i = 0; i < clist->length; i++) {
		s = clist->states[i];
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
	for (i = 0; i < l->length; i++) {
	    /* TODO: Use the enum with Match == 256. */
	    if (l->states[i]->c == '-')
	        return 1;
	}
	return 0;
}

int match(State *start, char *s)
{
	StateList *clist, *nlist, *temp;
	UNUSED(nlist);
	UNUSED(clist);
	UNUSED(temp);
    /* 
     * Allocate enough memory for two lists to keep track of the current states
     * of the simulated NFA.
     */
	clist = init_list(start, StateList_new());
	/*nlist = StateList_new();

	for (; *s; s++) {
		step(*s, clist, nlist);
		// Swap. 
		temp = clist;
		clist = nlist;
		nlist = temp;
	}
	return is_match(nlist);*/
	return 0;
}
