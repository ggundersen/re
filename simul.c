#include "simul.h"


/* 
 * This global keeps track of which NFA simulation we are on. Notice it is only
 * incremented when List_new() is called, which is only called when match() is
 * called. We then give every state in a simulation a lastlist of listid.
 */
int listid = 0;

/* These are preallocated lists we use to run the simulation. */
List l1, l2;

void add_state(List *l, State *s)
{
	if (s == NULL || s->lastlist == listid)
		return;
	s->lastlist = listid;
	/* TODO: Figure out enum! */
	if (s->c == 257) {
		/* Follow unlabeled arrows */
		add_state(l, s->out1);
		add_state(l, s->out2);
		return;
	}
	l->s[l->n++] = s;
}

List* List_new(State *s, List *l)
{
	listid++;
	l->n = 0;
	add_state(l, s);
	return l;
}

int is_match(List *l)
{
	int i;
	for (i = 0; i < l->n; i++)
	    /* TODO: Use the enum with Match == 256. */
	    if (l->s[i]->c == 256)
	        return 1;
	return 0;
}

void step(List *clist, int c, List *nlist)
{
	int i;
	State *s;
	listid++;
	nlist->n = 0;
	for (i = 0; i < clist->n; i++) {
		s = clist->s[i];
		if (s->c == c)
			add_state(nlist, s->out1);
	}
}

int match(State *start, char *s)
{
	List *clist, *nlist, *t;

	/* l1 and l2 are preallocated globals */
	clist = List_new(start, &l1);
	nlist = &l2;
	for(; *s; s++){
		step(clist, *s, nlist);
		t = clist; clist = nlist; nlist = t;	/* swap clist, nlist */
	}
	return is_match(clist);
}
