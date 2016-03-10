#include "genalg.h"


double exptime;
int s,c,r,e,nge,ex;
int printexperiment,printpopulation=0;
void mapletters();

void singleexperiment();
void etapa1();
void etapa2();
void etapa3();
void GAanalysis();
void (*GAexperiment)(void);


//algoritmo genético
Individual* genalg()
{
	Population nextgen;
	int i,j;

	initpopulation();
	best();

	//main loop
	for(i=0;i<MAXGENERATIONS && bestindividual->fitness>0;i++)
	{
		nextgen = nextgeneration();
		reinsertion(nextgen);
		best();
	}

	return bestindividual;
}


//executa genalg e imprime resultados
Individual* experiment()
{
	Individual* ind;
	struct timeval tim;
	int i,j;
	int values[3];
	char w[3][11];

	gettimeofday(&tim, NULL);  
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0); 

	ind = genalg();

	gettimeofday(&tim, NULL); 
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0); 

	exptime = t2-t1;

	//impressão dos resultados
	for(i=0;i<3;i++)
	{
		for(j=0;j<strlen(words[i]);j++)
		{
			w[i][j] = ind->traits[wordindexes[i][j]];
		}
		w[i][j] = '\0';
		values[i] = atoi(w[i]);
	}

	if(printpopulation)
	{
		printf("Final Population:\nIndividual\tFitness\n");
		for(i=0;i<POPSIZE;i++)
			printf("%s\t%d\n",population[i]->traits,population[i]->fitness);
	}

	if(printexperiment)
	{
		printf("\nExperiment\nExecution time: %f\nBest individual: %s\nFitness: %d\n\nValues\n%s + %s = %s",t2-t1,ind->traits,ind->fitness,words[0],words[1],words[2]);
		printf("\n%d + %d = %d\n--------------------------------\n\n",values[0],values[1],values[2]);
	}

	return ind;
}


//processa input.txt
void getinput()
{
	int i;
	FILE *f;

	Individual* (*sel[4])(void) = {&roullete,&tournament3,&tournament2,&stochevent};
	Individual* (*cross[2])(Individual*,Individual*) = {&cyclecrossover,&pmx};
	void (*reins[2])(Population) = {&bestreinsertion,&elitism};
	int (*eval[3])(Individual *ind) = {&evaluate1,&evaluate2,&evaluate3};
	Population (*ng[2])(void) = {&nextgenerationsimple,&nodoublesgeneration};
	void (*GA[5])(void) = {&singleexperiment,&etapa1,&etapa2,&etapa3,&GAanalysis};

	f = fopen("input.txt","r");
	i = fscanf(f,"S%d, C%d, R%d\n\n",&s,&c,&r);
	i = fscanf(f,"%s %s %s\n\n",words[0],words[1],words[2]);
	i = fscanf(f,"Population: %d\nGenerations: %d\nCrossover: %d%%\nMutation: %d%%\n",&POPSIZE,&MAXGENERATIONS,&NEXTGENSIZE,&MUTATIONRATE);
	i = fscanf(f,"Evaluation: %d\n",&e);
	i = fscanf(f,"Nextgen: %d\n",&nge);
	i = fscanf(f,"Experiment: %d\n",&ex);
	NEXTGENSIZE *= POPSIZE;
	NEXTGENSIZE /= 100;

	mapletters();

	selection = sel[s-1];
	crossover = cross[c-1];
	reinsertion = reins[r-1];
	evaluate = eval[e-1];
	nextgeneration = ng[nge-1];
	GAexperiment = GA[ex];
}


//mapeia as letras do problema aos índices de um vetor
void mapletters()
{
	int i,j,count=0;
	int letterindex[26];

	for(i=0;i<26;i++)
		letterindex[i] = -1;

	for(i=0;i<3;i++)
	{
		wordindexes[i] = malloc(strlen(words[i])*sizeof(int));
		for(j=0;j<strlen(words[i]);j++)
		{
			if(letterindex[words[i][j]-'A']==-1)
				letterindex[words[i][j]-'A'] = count++;
			wordindexes[i][j] = letterindex[words[i][j]-'A'];
		}

	}
	phenotypelength = count;
}


//realiza 1000 experimentos
void GAanalysis()
{
	int i,count,seed;
	double totaltime = 0;

	printexperiment = 1;

	seed = time(NULL);
	srand(seed);

	for(i=0,count=0;i<1000;++i)
	{
		if(experiment()->fitness==0)
			++count;
		totaltime += exptime;
	}
	printf("1000 experimentos\nConfiguração: S%d, C%d, R%d, A%d, Ind%d\nSolução encontrada %d vezes\nSeed: %d\n",s,c,r,e,nge,count,seed);
	printf("Tempo médio de execução: %f s\nTempo total: %f s\n",totaltime/1000,totaltime);
}


//realiza 1000 experimentos variando parâmetros
void etapa3()
{
	int i,count,seed;
	double totaltime = 0;
	int (*eval[3])(Individual *ind) = {&evaluate1,&evaluate2,&evaluate3};
	Individual* (*sel[4])(void) = {&roullete,&tournament3,&tournament2,&stochevent};
	Population (*ng[2])(void) = {&nextgenerationsimple,&nodoublesgeneration};

	seed = time(NULL);
	srand(seed);

	printexperiment = 0;

	for(e=1;e<=3;e++)
	{
		evaluate = eval[e-1];

		for(s=1;s<=4;s+=3)
		{
			selection = sel[s-1];

			for(nge=1;nge<=2;nge++)
			{
				nextgeneration = ng[nge-1];

				totaltime = 0;
				for(i=0,count=0;i<1000;i++)
				{
					if(experiment()->fitness==0)
						count++;
					totaltime += exptime;
				}
				printf("A%d, S%d, Ins%d\t%d\t%f\n",e,s,nge,count,totaltime);
			}
		}
	}
	
	printf("\nSeed: %d\n", seed);
}


//realiza 1000 experimentos variando parâmetros
void etapa2()
{
	int i,j,count,seed;
	double totaltime = 0;

	seed = time(NULL);
	srand(seed);

	printexperiment = 0;

	for(POPSIZE=50;POPSIZE<=200;POPSIZE+=POPSIZE)
	{
		for(MAXGENERATIONS=50;MAXGENERATIONS<=100;MAXGENERATIONS+=50)
		{
			for(j=0;j<=20;j+=20)
			{
				NEXTGENSIZE = 60 + j;
				NEXTGENSIZE *= POPSIZE;
				NEXTGENSIZE /= 100;

				for(MUTATIONRATE=10;MUTATIONRATE<=20;MUTATIONRATE+=10)
				{
					totaltime = 0;
					for(i=0,count=0;i<1000;++i)
					{
						if(experiment()->fitness==0)
							++count;
						totaltime += exptime;
					}
					printf("P%d, G%d, C%d, M%d\t%d\t%f\n",POPSIZE,MAXGENERATIONS,60+j,MUTATIONRATE,count,totaltime);
				}
			}
		}
	}
	
	printf("\nSeed: %d\n", seed);
}


//realiza 1000 experimentos variando configurações
void etapa1()
{
	int i,count,seed;
	double totaltime = 0;
	Individual* (*sel[3])(void) = {&roullete,&tournament3,&tournament2};
	Individual* (*cross[2])(Individual*,Individual*) = {&cyclecrossover,&pmx};
	void (*reins[2])(Population) = {&bestreinsertion,&elitism};

	seed = time(NULL);
	srand(seed);

	printexperiment = 0;

	for(r=1;r<=2;r++)
	{
		reinsertion = reins[r-1];

		for(c=1;c<=2;c++)
		{
			crossover = cross[c-1];

			for(s=1;s<=3;s++)
			{
				selection = sel[s-1];

				totaltime = 0;
				for(i=0,count=0;i<1000;i++)
				{
					if(experiment()->fitness==0)
						count++;
					totaltime += exptime;
				}
				printf("S%d, C%d, R%d\t%d\t%f\n",s,c,r,count,totaltime);
			}
		}
	}
	
	printf("\nSeed: %d\n", seed);
}


void singleexperiment()
{
	int i,count,seed;
	double totaltime = 0;

	printexperiment = 1;
	printpopulation = 1;

	seed = time(NULL);
	srand(seed);

	experiment();

	printf("Configuração: S%d, C%d, R%d, A%d, Ind%d\nSeed: %d\n",s,c,r,e,nge,seed);
}


int main()
{
	getinput();
	GAexperiment();


	// getinput();
	// Individual* ind = newindividual();
	// strcpy(ind->traits,"0649158723");
	// printf("%d\n",evaluate2(ind));


	// ind2 = newindividual();
	// printf("%s\t%d\n",ind->traits,evaluate(ind));
	// printf("%s\t%d\n",ind2->traits,evaluate(ind2));
	// ind = pmx(ind,ind2);
	// printf("\nFilhos:\n");
	// printf("%s\t%d\n",ind[0].traits,evaluate(&ind[0]));
	// printf("%s\t%d\n",ind[1].traits,evaluate(&ind[1]));



	// initpopulation();
	// nextgen = nextgeneration();

	// for(i=0;i<POPSIZE;i++)
	//   	printf("%s\t%d\n",population[i]->traits,population[i]->fitness);

	// printf("\nNext:\n");

	// for(i=0;i<NEXTGENSIZE;i++)
	//  	printf("%s\t%d\n",nextgen[i]->traits,nextgen[i]->fitness);

	//  printf("\nReinsertion:\n");
	//  reinsertion(nextgen);
	//  for(i=0;i<POPSIZE;i++)
	//   	printf("%s\t%d\n",population[i]->traits,population[i]->fitness);



	// for(i=0;i<POPSIZE;i++)
	//  	printf("%s\t%d\n",population->at(i).traits,population->at(i).fitness);

	// ind = new Individual[2];
	// strcpy(ind[0].traits,"75143682");
	// strcpy(ind[1].traits,"34875261");

	// ind = cyclecrossover(&ind[0],&ind[1]);

	// printf("%s\n%s\n",ind[0].traits,ind[1].traits);


	// vector<Individual>* population = initialpopulation();
	// for(i=0;i<INITIALPOP;i++)
	// 	printf("%s\t%d\n",population->at(i).traits,population->at(i).fitness);
	// ind = tournament(population);
	// printf("\n%s\t%d\n",ind->traits,ind->fitness);

	//for(j=0;j<500;j++)
	//{
	//	i = genalg();
	//	printf("%s\t%d\n",i->traits,i->fitness);
	//	if(i->fitness==0)
	//		count++;
	//}
	//printf("1000 experimentos\nSolução encontrada %d vezes\n",count);



	// while(flag=='y')
	// {
	// 	i = genalg();
	// 	printf("sendmory = %s\n\n",i->traits);

	// 	char send[5] = {i->traits[S],i->traits[E],i->traits[N],i->traits[D],'\0'};
	// 	char more[5] = {i->traits[M],i->traits[O],i->traits[R],i->traits[E],'\0'};
	// 	char money[6] = {i->traits[M],i->traits[O],i->traits[N],i->traits[E],i->traits[Y],'\0'};

	// 	printf("send + more = money\n%s + %s = %s\n",send,more,money);
	// 	printf("\ncontinue?(y/n)\n");
	// 	flag = getchar();
	// 	getchar();
	// }
	return 0;
}