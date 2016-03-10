

typedef struct I Individual;
struct I
{
		char traits[11]; //valores para sendmory
		int fitness; //aptidão do indivíduo (menor é melhor)
};

typedef Individual** Population;


struct l
{
	int size;
	char *info;
};
typedef struct l* list;


struct pl
{
	Population info;
	int size;
};
typedef struct pl* poplist;