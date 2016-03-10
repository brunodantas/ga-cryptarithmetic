#include "genalg.h"


//gera dois indivíduos filhos dados dois pais
Individual* cyclecrossover(Individual *p1,Individual *p2)
{
	int i,j,k,inicial,aux;
	Individual *c = malloc(2*sizeof(Individual));

	strcpy(c[0].traits,p1->traits);
	strcpy(c[1].traits,p2->traits);

	k = rand()%10;
	aux = c[0].traits[k];
	c[0].traits[k] = c[1].traits[k];
	c[1].traits[k] = aux;

	inicial = c[1].traits[k];
	i = c[0].traits[k];
	while(i!=inicial)
	{
		for(j=0;j<10;j++)
		{
			if(j!=k && c[0].traits[j]==i)
				break;
		}
		aux = c[0].traits[j];
		c[0].traits[j] = c[1].traits[j];
		c[1].traits[j] = aux;
		k = j;
		i = c[0].traits[j];
	}
	return c;
}


//gera dois indivíduos filhos dados dois pais
Individual* pmx(Individual *p1,Individual *p2)
{
	Individual *c = malloc(2*sizeof(Individual));
	list digits;
	int a,b,d1,d2,i,j;
	int occur[10];
	for(i=0;i<10;i++)
		occur[i] = 0;
	strcpy(c[0].traits,p1->traits);
	strcpy(c[1].traits,p2->traits);

	a = rand()%10;
	b = rand()%(10-a) + a; // a <= b < 10

	for(i=a;i<=b;i++)
		c[0].traits[i] = p2->traits[i];
	for(i=a;i<=b;i++)
		c[1].traits[i] = p1->traits[i];

	//contagem de ocorrências de cada dígito
	for(i=0;i<10;i++)
		occur[c[0].traits[i]-'0']++;

	//tratamento de repetições
	for(i=a;i<=b;i++)
	{
		d1 = c[0].traits[i];
		d2 = c[1].traits[i];
		for(j=0;j<10;j++)
		{
			if(j==a)
			{
				j = b;
				continue;
			}
			if(c[0].traits[j]==d1)
			{
				c[0].traits[j] = d2;
				i = a-1;
				break;
			}
			if(c[1].traits[j]==d2)
			{
				c[1].traits[j] = d1;
				i = a-1;
				break;
			}
		}
	}
	return c;
}