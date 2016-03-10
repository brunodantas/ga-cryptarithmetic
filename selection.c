#include "genalg.h"


//"evento estocástico"
Individual* stochevent()
{
	Individual *ind;
	int i,r, fitness;

	fitness = population[POPSIZE-1]->fitness + population[0]->fitness;

	i = rand()%POPSIZE;
	while(1)
	{
		ind = population[i];
		r = rand() % fitness;
		if(r >= ind->fitness)
			return ind;
		i = (i+1)%POPSIZE;
	}
}


//seleciona um indivíduo para reprodução
Individual* roullete()
{
	int i;
	int worstfitness;
	Individual *p;
	long j, roulletesize = 0;

	worstfitness = population[POPSIZE-1]->fitness + 1;

	for(i=0;i<POPSIZE;++i)
		roulletesize += worstfitness - population[i]->fitness;

	j = rand()%roulletesize;
	for(i=0;i<POPSIZE;++i)
	{
		p = population[i];
		j -= worstfitness - p->fitness;
		if(j<0)
			break;
	}
	return p;
}

// Individual* roullete()
// {
// 	int i;
// 	int acc = 0;
// 	int worstfitness;
// 	Individual *p;
// 	long j, roulletesize = 0;

// 	worstfitness = population[POPSIZE-1]->fitness + 1;
// 	roulletesize = worstfitness * POPSIZE;

// 	j = rand()%roulletesize + 1;
// 	while(1)
// 	{
// 		for(i=0;i<POPSIZE;i++)
// 		{
// 			p = population[i];
// 			j -= worstfitness - p->fitness;
// 			acc += worstfitness - p->fitness;
// 			if(j<0)
// 				return p;
// 		}
// 		j = j%acc;
// 	}
// }


//seleciona um indivíduo para reprodução
Individual* tournament(int tournamentsize)
{
	Individual *best,*ind;
	int i,r;

	r = rand()%POPSIZE;
	best = population[r];

	for(i=1;i<tournamentsize;++i)
	{
		r = rand()%POPSIZE;
		ind = population[r];
		if(ind->fitness < best->fitness)
			best = ind;
	}
	return best;
}


Individual* tournament3()
{
	return tournament(3);
}


Individual* tournament2()
{
	return tournament(2);
}