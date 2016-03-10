#include "genalg.h"


list newlist(int cap)
{
	list l = malloc(sizeof(struct l));
	l->size = 0;
	l->info = malloc(cap*sizeof(char));
	return l;
}

void add(list l,char a)
{
	l->info[l->size] = a;
	l->size++;
}

char at(list l,int index)
{
	return l->info[index];
}

void erase(list l,int index)
{
	l->info[index] = l->info[l->size-1];
	l->size--;
}