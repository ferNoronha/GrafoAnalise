#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

void exibeMat(int **mat,int lin,int col)
{
	for(int i=0;i<lin;i++)
	{
		for(int j=0;j<col;j++)
			printf("%d\t",mat[i][j]);
		printf("\n");
	}
}

void geraMatAnalise(int **mat, int **busca, int **matAnalise, int tam)
{
	system("cls");
	printf("Matriz\n");
	exibeMat(mat,tam,tam);
	printf("\nBusca\n");
	exibeMat(busca,tam,tam);
	
	//gera nova matriz
	int **novamat = new int*[tam],i,j, pos, prenum, maior=-1, cont, menor=99, vertices[tam];
	bool biconexo = true;
	
	for(i=0;i<tam;i++)
		novamat[i] = new int[tam];
	for(i=0;i<tam;i++)
		for(j=0;j<tam;j++)
		{
			novamat[i][j] = 0;
			vertices[j] = 0;
		}
	
	//insere os vértices q podem ter outro tipo de ligação
	for(i=0;i<tam;i++)
		for(j=0;j<tam;j++)
			if(busca[i][j]==0 && mat[i][j]==1)
				novamat[i][j]=1;
	
	printf("Nova Matriz\n");
	exibeMat(novamat,tam,tam);	
				
	//percorre a nova matriz para ver o caminho alternativo mais proximo
	for(i=0;i<tam;i++)
		for(j=0;j<tam;j++)
			if(novamat[i][j]==1)
			{//armazena o prenum do prossivel possimo numero na matriz de analise
				matAnalise[i][1] = matAnalise[j][0];
				j=tam;
			}
	//começa o pós ordem a partir do maior prenum
	for(i=0;i<tam;i++)
		if(matAnalise[i][0]>maior)
		{
			maior=matAnalise[i][0];
			prenum = maior;
			pos=i;
		}
	
	while(prenum>0)
	{
		cont=0;
			for(j=0;j<tam;j++)
			{
				if(busca[pos][j]==1)
					cont++;
			}
			//percorre a matriz de busca para verificar se é folha, se for folha calcula o menor numero na matriz
			// se nao for folha, verifica o menor dos filhos
			if(cont==1)
			{//se for folha, já verifica qual o menor
				if(matAnalise[pos][0]<matAnalise[pos][1])
					matAnalise[pos][3] = matAnalise[pos][0];
				else
					matAnalise[pos][3] = matAnalise[pos][1];
			}
			else
			{//se não for, percorre os filhos para ver qual dos filhos é o menor
				for(j=0;j<tam;j++)
					if(mat[pos][j]==1 && matAnalise[pos][0]<matAnalise[j][0] && matAnalise[j][3]<menor)
						menor = matAnalise[j][3];
				matAnalise[pos][2] = menor;
				//depois verifica qual dos numeros gerados é de fato o menor
				if(matAnalise[pos][0]<matAnalise[pos][1])
				{
					if(matAnalise[pos][0]<matAnalise[pos][2])
						matAnalise[pos][3] = matAnalise[pos][0];
					else
						matAnalise[pos][3] = matAnalise[pos][2];
				}
				else
				{
					if(matAnalise[pos][1]<matAnalise[pos][2])
						matAnalise[pos][3] = matAnalise[pos][1];
					else
						matAnalise[pos][3] = matAnalise[pos][2];
				}
			}
			prenum--;
			for(i=0;i<tam;i++)
				if(prenum == matAnalise[i][0])
					pos=i;
	}
	for(i=1;i<tam;i++)
	{
		cont=0;
		for(j=0;j<tam;j++)
		{
			if(busca[pos][j]==1)
				cont++;
		}
		if(cont==1)
		{
			if(matAnalise[i][3]>=matAnalise[i][0])
			{
				biconexo = false;
				j=tam;
				vertices[i] = 1;
			}
		}
	}
	
	for(i=0;i<tam;i++)
		for(j=1;j<3;j++)
			if(matAnalise[i][j]==500)
				matAnalise[i][j]=0;
	printf("Matriz de Analise\n");
	exibeMat(matAnalise,tam,4);
	getch();
	system("cls");
	if(biconexo)
		printf("\nBiconexo");
	else
	{
		printf("Ponto(s) de articulacao");
		for(i=0;i<tam;i++)
			if(vertices[i]!=0)
			{
				printf("\nVertice: %d",i+1);
			}
	}
	getch();
}

int main()
{
	int tam=0,i,j,num, soma, somaT, somaGraus=0;
	char lin[30], linha[30],letra,prox;
	FILE *Arq = fopen("Matriz_Teste_PontoArticulacao.txt","r");
	fgets(lin,30,Arq);
	for(i=0; i<30 && lin[i]!='\n'; i++)
		if(lin[i]!=' ')
			if(lin[i+1]==' ' || lin[i+1]=='\n')
				tam++;
	int **mat= new int*[tam];
	for(i=0;i<=tam;i++)
		mat[i] = new int[tam+1];
		
	j=i=0;
	while(!feof(Arq))
	{
		letra = fgetc(Arq);
		if(letra!='\n')
		{
			prox = fgetc(Arq);
			if(prox==9 || prox=='\n')
			{
				num = letra-48;
			}
			else
			{
				num = (letra-48)*10+prox-48;
				prox = fgetc(Arq);
			}
			mat[i][j] = num;
			j++;
		}
		else
		{
			i++;
			j=0;
		}
		if(prox=='\n')
		{
			i++;
			j=0;
		}
	}
	
	exibeMat(mat,tam,tam);
	
	bool grafo = true;
	for(i=0;i<tam;i++)
		for(j=0;j<tam;j++)
		{
			if(i!=j)
				if(mat[i][j]!=mat[j][i])
					grafo = false;
		}
	if(grafo)
		printf("\n\nGrafo");
	else
		printf("\n\nDigrafo");
	
	bool simples = true;
	for(i=0,j=0;i<tam;i++,j++)
		if(mat[i][j]!=0)
			simples = false;
	
	if(simples)
		printf("\nSimples");
	else
		printf("\nNao e simples");
	
	
	bool regular=true;
	for(i=0;i<tam;i++)
	{
		soma=0;
		for(j=0;j<tam;j++)
			if(mat[i][j]!=0)
				soma++;
		mat[i][j]=soma;
		if(i==0)
			somaT=soma;
		else
		{
			if(soma!=somaT)
			{
				regular = false;
				i=tam;
			}
		}
	}
	if(!grafo && !regular)
	{
		for(i=0;i<tam;i++)
		{
			soma=0;
			for(j=0;j<tam;j++)
				if(mat[j][i]!=0)
					soma++;
			if(i==0)
				somaT=soma;
			else
			{
				if(soma!=somaT)
				{
					regular = false;
					i=tam;
				}
			}
		}
	}
	
	if(regular)
		printf("\n%d - Regular", somaT);
	else
		printf("\nIrregular");
		
	if(simples && regular)
	{
		for(i=0,j=tam;i<tam;i++)
			somaGraus += mat[i][j];
		if(tam*(tam-1)==somaGraus)
			printf("\nK-%d - Completo", tam);
		else
			printf("\nIncompleto");
	}
	else
		printf("\nIncompleto");
		
	
	int **busca = new int*[tam], vLigados[tam], pilha[tam], tl=0, prenum[tam], cont=2, **matAnalise = new int*[tam];
	for(i=0;i<tam;i++)
	{
		matAnalise[i] = new int [4];
		busca[i] = new int[tam];
		vLigados[i]=0;
		prenum[i] = 0;
		matAnalise[i][1] = matAnalise[i][2] = 500;
	}
	
	for(i=0; i<tam; i++)
		for(j=0; j<tam; j++)
			busca[i][j]=0;
	vLigados[0] = 1;
	prenum[0]=1;
	
	printf("\n\nAperte qualquer tecla para a Busca em Profundidade!...");
	getch();
	system("cls");
	
	if(grafo)
	{
		i=0;
		pilha[tl++] = i;
		while(tl>0)
		{
			for(j=0; j<tam; j++)
			{
				if(mat[i][j]!=0 && vLigados[j]==0)
				{
					busca[i][j] = 1;
					busca[j][i] = 1;
					vLigados[j]=1;
					pilha[tl++]=i;
					if(prenum[j]==0)
					{
						prenum[j] = cont++;
						matAnalise[j][0] = prenum[j];
					}
					i=j;
					j=0;
				}
			}
			i = pilha[--tl];
		}
	}
	else
	{
		i=0;
		pilha[tl++] = i;
		while(tl>0)
		{
			for(j=0; j<tam; j++)
			{
				if(mat[i][j]!=0 && vLigados[j]==0)
				{
					busca[i][j] = 1;
					vLigados[j]=1;
					pilha[tl++]=i;
					i=j;
					j=0;
				}
			}
			i = pilha[--tl];
		}
	}
	for(i=0; i<tam; i++)
	{
		for(j=0; j<tam; j++)
			printf("%d\t",busca[i][j]);
		printf("\n");
	}
	printf("\n\n");
	for(i=0;i<tam;i++)
		printf("%d ",prenum[i]);
		
	/*printf("\nAperte qualquer tecla para a Busca em Largura!...");
	getch();
	system("cls");
	
	for(i=0; i<tam; i++)
		for(j=0; j<tam; j++)
			busca[i][j]=0;
	for(i=0; i<tam; i++)
		vLigados[i]=0;
		
	tl=0;
	i=0;
	pilha[tl++] = i;
	while(i<tam)
	{
		for(j=0; j<tam; j++)
		{
			if(mat[i][j]!=0 && vLigados[j]==0)
			{
				busca[i][j]=1;
				pilha[tl++]=j;
				vLigados[j]=1;
			}
		}
		i++;
	}*/
	if(grafo)
	{
		for(i=0;i<tam;i++)
			matAnalise[i][0] = prenum[i];
		geraMatAnalise(mat,busca,matAnalise,tam);
	}
}
