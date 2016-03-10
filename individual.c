#include "genalg.h"


//calcula e seta aptidão do indivíduo
int evaluate1(Individual *ind)
{
	int i,j;
	int values[3];
	char w[3][11];

	for(i=0;i<3;++i)
	{
		for(j=0;j<strlen(words[i]);++j)
		{
			w[i][j] = ind->traits[wordindexes[i][j]];
		}
		w[i][j] = '\0';
		values[i] = atoi(w[i]);
	}

	ind->fitness = abs(values[0] + values[1] - values[2]);
	return ind->fitness;
}


int digit(int number,int index)
{
	for(;index>0;index--)
		number /= 10;
	return number%10;
}


//calcula e seta aptidão do indivíduo
int eval23(Individual *ind,int flag)
{
	int i,j,p,l=0;
	int d[3],a[10];
	int carry=0,sum=0;
	int values[3];
	char w[3][11];

	for(i=0;i<10;++i)
		a[i] = 0;

	for(i=0;i<3;++i)
	{
		for(j=0;j<strlen(words[i]);++j)
		{
			w[i][j] = ind->traits[wordindexes[i][j]];
		}
		w[i][j] = '\0';
		values[i] = atoi(w[i]);
		if(strlen(words[i])>l)
			l = strlen(words[i]);
	}

	ind->fitness = 0;
	p = 1;
	for(i=0;i<l+1;++i)
	{
		for(j=0;j<3;++j)
			d[j] = digit(values[j],i);

		if((d[0] + d[1] + carry)%10 - d[2] != 0)
			ind->fitness++;

		if(flag)
		{
			sum += ((d[0] + d[1] + carry)%10 - d[2]) * p;
			p *= 10;
		}

		carry = (d[0] + d[1] + carry)/10;
	}

	if(flag)
		ind->fitness *= abs(sum);

	return ind->fitness;
}


int evaluate2(Individual *ind)
{
	return eval23(ind,0);
}


int evaluate3(Individual *ind)
{
	return eval23(ind,1);
}


//gera um indivíduo válido
Individual* newindividual()
{
	Individual* ind = malloc(sizeof(Individual));
	list digits = newlist(10);
	int i,j;

	ind->traits[10] = '\0';
	for(i=0;i<10;++i)
		add(digits,'0'+i);

	for(i=0;i<10;++i)
	{
		j = rand() % digits->size;
		ind->traits[i] = at(digits,j);
		erase(digits,j);
	}

	return ind;
}


//troca dois genes de lugar
void mutation(Individual *ind)
{
	char temp;
	int a,b;
	a = rand()%10;
	b = a;
	for(b=a;b==a;b = rand()%10);
	temp = ind->traits[a];
	ind->traits[a] = ind->traits[b];
	ind->traits[b] = temp;
}


void phenotypemutation(Individual *ind)
{
	char temp;
	int a,b;
	a = rand()%phenotypelength;
	b = a;
	for(b=a;b==a;b = rand()%10);
	temp = ind->traits[a];
	ind->traits[a] = ind->traits[b];
	ind->traits[b] = temp;
}