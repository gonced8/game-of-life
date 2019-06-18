#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define SPEED 10

#define COLUNAS 60
#define LINHAS 45
#define VIVO '*'
#define MORTO ' '
#define OCUPACAO 30 /* min: 0	max: 100 */

typedef struct Posicao
{
	int x, y;
	char estado;
}POS;

int definicoes (int * colunas, int *linhas, int * ocupacao);
int inicia_mem (POS * * quadro, POS * * quadro2, int colunas, int linhas, int * celulas_vivas, int ocupacao);
int imprime_quadro (POS * quadro, int colunas, int linhas, int celulas_vivas);
int atualiza(POS * quadro, POS * quadro2, int colunas, int linhas, int * celulas_vivas);
int copia_quadros (POS * quadro, POS * quadro2, int n);
int ler_ficheiro (int * colunas, int *linhas, POS * * quadro, POS * * quadro2, int * celulas_vivas, int argc, char *argv[]);


int main (int argc, char *argv[])
{
	int colunas, linhas, i=0;
	int ocupacao;
	POS *quadro, *quadro2;
	int celulas_vivas=0;
	char input[2]={'\0'};
	
	printf("\nBemvindo ao programa GAME OF LIFE.\n");
	
	if(argc==1)
	{
		printf("\nPara gerar um quadro aleatório prima <ENTER>.\nPara ler do ficheiro prima <SPACEBAR><ENTER>\n");
	
		do
		{
			input[i] = getchar();
			
			i=i+1;
		}while (input[i-1]!='\n' && i<2);
	}
	
	if (input[0]==' ' || argc!=1)
	{	if(ler_ficheiro(&colunas, &linhas, &quadro, &quadro2, &celulas_vivas, argc, argv)==1)
			return 1;
	}
	else
	{
		definicoes (&colunas, &linhas, &ocupacao);
		inicia_mem (&quadro, &quadro2, colunas, linhas, &celulas_vivas, ocupacao);
	}
	
	printf("\nPrima ENTER para começar.");
	getchar();
	
	imprime_quadro (quadro, colunas, linhas, celulas_vivas);
	
	getchar();
	
	while(1)
	{
		atualiza(quadro, quadro2, colunas, linhas, &celulas_vivas);
		imprime_quadro (quadro, colunas, linhas, celulas_vivas);
		fflush(stdout);
		usleep(1000000/SPEED);		
	}
	
	free(quadro);
	printf("\nFIM\n");
	exit(0);
}



int definicoes (int * colunas, int *linhas, int *ocupacao)
{
	int i=0, j=0;
	char input[12]={'\0'};
	
	*colunas=COLUNAS;
	*linhas=LINHAS;
	*ocupacao=OCUPACAO;
	
	printf("\nInsira o número de colunas seguido do número de linhas e da ocupacao(%%).\nPrima ENTER para utilizar a pré-definição: > %d %d %d\n\n", COLUNAS, LINHAS, OCUPACAO);
	
	do
	{
		input[i] = getchar();
		
		i=i+1;
	}while (input[i-1]!='\n');
	
	
	if (input[0]!='\n')
	{
		for(i=0; i<12; i=i+1)
		{
			if(input[i]==' ')
			{
				if(j==0 && i!=0)
					j=i;
				else
					break;
			}			
		}
		
		input[i] = '\0';
		input[j] = '\0';
		
		*colunas = atoi (input+0);
		*linhas = atoi (input+j+1);
		*ocupacao = atoi (input+i+1);
	}
	
	printf("Colunas: %d\tLinhas: %d\tOcupação: %d\n", *colunas, *linhas, *ocupacao);
	
	return 0;
}



int inicia_mem (POS * * quadro, POS * * quadro2, int colunas, int linhas, int * celulas_vivas, int ocupacao)
{
	int i, j;
	int r = 0;
	
	*celulas_vivas=0;
	
	srand(time(NULL));
	
	*quadro = (POS *) calloc (colunas*linhas, sizeof(POS));
	
	*quadro2 = (POS *) calloc (colunas*linhas, sizeof(POS));
	
	for (i=0; i<linhas; i=i+1)
	{
		for(j=0; j<colunas; j=j+1)
		{
			r = (rand()%(100) +1);
			
			if(r>ocupacao)
				(*quadro+(i*colunas+j))->estado = MORTO;
			else
			{
				(*quadro+(i*colunas+j))->estado = VIVO;
				*celulas_vivas = *celulas_vivas + 1;
			}			
		}
	}
	
	return 0;
}



int imprime_quadro (POS * quadro, int colunas, int linhas, int celulas_vivas)
{
	int i, j;


	system("clear");
/*
	clear();
*/

	for (i=0; i<linhas; i=i+1)
	{
		for(j=0; j<colunas; j=j+1)
		{
			putchar((quadro+(i*colunas+j))->estado);
			putchar(' ');
			fflush(stdout);
		}
		
		putchar('\n');
		fflush(stdout);
	}
	
	printf("\nCélulas vivas: %d\t(%d%%)", celulas_vivas, (int)(celulas_vivas*100.0/(linhas*colunas)));
	fflush(stdout);
	
	return 0;
}



int atualiza(POS * quadro, POS * quadro2, int colunas, int linhas, int * celulas_vivas)
{
	int i, j, k, h, a, b;	/* k->a ; h->b */
	int vizinhos = 0;
	
	*celulas_vivas = 0;
	
	copia_quadros(quadro2, quadro, colunas*linhas);
	
	for (i=0; i<linhas; i=i+1)	/* Analisa o interior do quadro */
	{
		for(j=0; j<colunas; j=j+1)
		{
			vizinhos = 0;
			
			for (k=-1; k<=1 && vizinhos<=3; k=k+1)
			{
				a=k;
				/* Condições para as arestas horizontais */
				if(i==0 && k==-1)
					a=linhas-1;
	
				else if(i==linhas-1 && k==1)
					a= (-linhas) + 1;
				
				
				for (h=-1; h<=1 && vizinhos<=3; h=h+1)
				{
					b=h;
					/* Condição para o caso de analisar na posição da célula */
					if (a==0 && b==0)
						continue;
					
					/* Condições para as arestas verticais */
					if(j==0 && h==-1)
						b=colunas-1;						
					
					else if (j==colunas-1 && h==1)
						b= (-colunas) + 1;
					
					/* Condição para o caso de analisar na posição da célula */
					if (a==0 && b==0)
						continue;
					

					if( ( quadro + ((i+a)*colunas+(j+b)) )->estado == VIVO )
						vizinhos = vizinhos + 1;
					
				}
			}
			
			if (vizinhos<2)	/* Under population */
				(quadro2+(i*colunas+j))->estado = MORTO;
			
			else if (vizinhos>3)	/* Over population */
				(quadro2+(i*colunas+j))->estado = MORTO;
				
			else if (vizinhos==3)	/* Reproduction */
				(quadro2+(i*colunas+j))->estado = VIVO;
			
			if ((quadro2+(i*colunas+j))->estado == VIVO)
				*celulas_vivas = *celulas_vivas + 1;
				
		}

	}
	
	copia_quadros (quadro, quadro2, colunas*linhas);
	
	return 0;
}



int copia_quadros (POS * quadro, POS * quadro2, int n)
{
	int i;
	
	for(i=0; i<n; i=i+1)
		quadro[i] = quadro2[i];
	
	return 0;
}



int ler_ficheiro (int * colunas, int *linhas, POS * * quadro, POS * * quadro2, int * celulas_vivas, int argc, char *argv[])
{
	FILE * fp = NULL;
	char linha[99]={'\0'};
	int i=0, j=0, k=0;
	
	if(argc==2)
		fp = fopen (argv[1], "r");
	
	if(fp==NULL || argc==1)
	{
		if(argc==2)
			printf("\nErro ao abrir o ficheiro %s. Tentar-se-á abrir o definicoes.txt\n", argv[1]);
		fp = fopen ("definicoes.txt", "r");
	}
	
	if (fp == NULL)
	{
		printf("\nErro ao abrir o ficheiro de texto!\n");
		return 1;
	}
	
	while(feof(fp)==0)
	{
		fgets(linha, 99, fp);
		i=i+1;
	}
	
	fclose(fp);
	
	*colunas = strlen(linha);
	*linhas = i;
	
	printf("Colunas: %d\tLinhas: %d\n", *colunas, *linhas);
	
	for(i=0; i<99; i=i+1)
		linha[i]='\0';
	
	
	if(argc==2)
		fp = fopen (argv[1], "r");
	
	if(fp==NULL || argc==1)
		fp = fopen ("definicoes.txt", "r");
	
	
	*quadro = (POS *) calloc ((*colunas) * (*linhas), sizeof(POS));
	*quadro2 = (POS *) calloc ((*colunas) * (*linhas), sizeof(POS));
	
	for(i=0; i<(*linhas); i=i+1)
	{
		fgets(linha, 99, fp);
		
		/* Vê quando começa a string pois o ficheiro tem "ruído" na primeira linha */
		for(k=0; linha[k]!='0' && linha[k]!='1'; )
		{
			printf("%c", linha[k]);
			fflush(stdout);
			k=k+1;
		}

		for(j=0 ; j<(*colunas) && k<99 && linha[k]!='\n'; j=j+1)
		{
			if(linha[k]=='0')
			{
				(*quadro+(i*(*colunas)+j))->estado = MORTO;
			}
			else if (linha[k]=='1')
			{
				(*quadro+(i*(*colunas)+j))->estado = VIVO;
				*celulas_vivas = *celulas_vivas + 1;
			}
			
			k=k+1;
		}
	}

	fclose(fp);
	
	return 0;
}
