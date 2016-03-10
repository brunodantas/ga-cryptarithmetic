#ifndef GENALG_H
#define GENALG_H


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>  
#include "datastructures.h"


//variáveis globais

int POPSIZE;
int NEXTGENSIZE;
int MUTATIONRATE; //percent
int MAXGENERATIONS;
Population population;
Individual *bestindividual;
char words[3][11];
int *wordindexes[3];
Individual* (*selection)(void);
Individual* (*crossover)(Individual*,Individual*);
void (*reinsertion)(Population);
int (*evaluate)(Individual *ind);
Population (*nextgeneration)(void);
int phenotypelength;


//declarações de funções

Individual* cyclecrossover(Individual *p1,Individual *p2);
Individual* pmx(Individual *p1,Individual *p2);

Individual* stochevent();
Individual* roullete();
Individual* tournament3();
Individual* tournament2();

void bestreinsertion(Population nextgen);
void elitism(Population nextgen);

void best();
Population nextgenerationsimple();
Population nodoublesgeneration();
void populationinsert(Population population,Individual *ind,int size);
Population* initpopulation();

int evaluate1(Individual *ind);
int evaluate2(Individual *ind);
int evaluate3(Individual *ind);
Individual* newindividual();
void mutation(Individual *ind);
void phenotypemutation(Individual *ind);

list newlist(int cap);
void add(list l,char a);
char at(list l,int index);
void erase(list l,int index);


#endif