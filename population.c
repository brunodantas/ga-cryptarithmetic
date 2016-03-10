#include "genalg.h"


//gera população inicial
Population* initpopulation()
{
	Individual* ind;
	population = malloc((POPSIZE+NEXTGENSIZE)*sizeof(Individual*));
	int i,j;
	for(i=0;i<POPSIZE;++i)
	{
		ind = newindividual();
		for(j=0;j<i;++j)
		{
			if(strcmp(ind->traits,population[j]->traits)==0)
			{
				mutation(ind);
				j = -1;
			}
		}
		populationinsert(population,ind,i);
	}
	return &population;
}


//insere indivíduo ordenado pela aptidão
void populationinsert(Population population,Individual *ind,int size)
{
	int pos,i;
	evaluate(ind);
	for(pos=0;pos<size;++pos)
	{
		if(population[pos]->fitness > ind->fitness)
			break;
	}
	for(i=size;i>pos;i--)
		population[i] = population[i-1];

	population[pos] = ind;
}


//retorna 0 se fenótipos são idênticos
int comparephenotype(Individual* ind1,Individual* ind2)
{
	int i;

	for(i=0;i<phenotypelength;++i)
	{
		if(ind1->traits[i]!=ind2->traits[i])
			return 1;
	}

	return 0;
}


//retorna um vetor de indivíduos descendentes da população atual
Population nextgenerationsimple()
{
	int couples = 0;
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int flag,i,r;

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = selection();
		p2 = selection();
		ind = crossover(p1,p2);
		ind2 = &ind[1];

		//mutação
		r = rand()%100;
		if(r<MUTATIONRATE && evaluate(ind)!=0)
			mutation(ind);
		r = rand()%100;
		if(r<MUTATIONRATE && evaluate(ind2)!=0)
			mutation(ind2);

		populationinsert(nextgen,ind,i);
		populationinsert(nextgen,ind2,i+1);
	}
	return nextgen;
}


//retorna um vetor de indivíduos descendentes da população atual
Population nodoublesgeneration()
{
	int couples = 0;
	char buff[11];
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int flag,i,j,r;

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = selection();
		p2 = selection();
		ind = crossover(p1,p2);
		ind2 = &ind[1];

		//mutação
		r = rand()%100;
		if(r<MUTATIONRATE && evaluate(ind)!=0)
			mutation(ind);
		r = rand()%100;
		if(r<MUTATIONRATE && evaluate(ind2)!=0)
			mutation(ind2);

		// while(comparephenotype(ind,p1)==0 || comparephenotype(ind,p2)==0)
		// 	phenotypemutation(ind);

		// while(comparephenotype(ind2,p1)==0 || comparephenotype(ind2,p2)==0 || comparephenotype(ind,ind2)==0)
		// 	phenotypemutation(ind2);

		for(j=0;j<POPSIZE+i;++j)
		{
			if(comparephenotype(ind,population[j])==0)
			{
				phenotypemutation(ind);
				j=-1;
				continue;
			}
			if(comparephenotype(ind2,population[j])==0)
			{
				phenotypemutation(ind2);
				j=-1;
				continue;
			}
			if(comparephenotype(ind2,ind)==0)
			{
				phenotypemutation(ind2);
				j=-1;
				continue;
			}
		}
		populationinsert(nextgen,ind,i);
		populationinsert(nextgen,ind2,i+1);
	}
	return nextgen;
}


//encontra o melhor individuo da população
void best()
{
	int i;
	bestindividual = population[0];

	for(i=i;i<POPSIZE;++i)
	{
		if(population[i]->fitness < bestindividual->fitness)
			bestindividual = population[i];
	}
}