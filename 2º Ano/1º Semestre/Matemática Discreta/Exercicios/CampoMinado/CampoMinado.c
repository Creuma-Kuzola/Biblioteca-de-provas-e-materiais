/*-------------------------------------------------------------------------------------------------------
	Autor: Edson Gregorio
	Projecto: Campo Minado
	Data: 4/06/2019;
	Efeito: Trabalho de Tecnicas de Programacao
			(Engenharia Informatica - Universidade Catolica de Angola) - Prof. Manuel Menezes
--------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

/* CONSTANTES */
#define MMAX 90
#define NMAX 90
#define SMAX 65535 
#define MINA -1

#define PERDEU 10
#define GANHOU 11
#define CONTINUA 12
#define SEM_EFEITO 13
#define ABERTA 14
#define MARCADA 15
#define DESMARCADA 16

typedef enum {FALSE = 0, TRUE = 1} Boolean;

typedef struct 
{
	int tabuleiroPrincipal[MMAX][NMAX];
	int tabuleiroAuxiliar[MMAX][NMAX];
	int posMarcadas;
	int posAbertas;	
} CAMPO_MINADO;

/*--------------------------------------------------------------------------------------------------------------------
    	Prototipo de Funcoes
----------------------------------------------------------------------------------------------------------------------*/
int lerNum(int min, int max);
void telaDeInicio();
void inicializarTabuleiro(CAMPO_MINADO *campoMinado, int m, int n, int i, int j);
void imprimirColunas(int i, int n);
void imprimirElemento(CAMPO_MINADO campoMinado, int m, int n, int i, int j, Boolean perdeu, Boolean ganhou);
void imprimirCampo(CAMPO_MINADO campoMinado, int m, int n, Boolean perdeu, Boolean ganhou);
void preencherCampo(CAMPO_MINADO *campoMinado, int m, int n, int z, int i, int j);
void distribuirMinas(CAMPO_MINADO *campoMinado, int m, int n, int z);
void jogo(CAMPO_MINADO *campoMinado, int m, int n,  int z, Boolean *perdeu, Boolean *ganhou);
int lerJogada(CAMPO_MINADO *campoMinado, int m, int n,  int z, Boolean *perdeu, Boolean *ganhou);
int abrirPosicao(CAMPO_MINADO *campoMinado, int i, int j, int m, int n, int z, Boolean *perdeu, Boolean *ganhou);
void abrirAdjacentes(CAMPO_MINADO *campoMinado, int i, int j, int m, int n, int deltaL[], int deltaC[], int k);

/*---------------------------------------------------------------------------------------------------------------------------------------------
		FUNCAO PRINCIPAL
-----------------------------------------------------------------------------------------------------------------------------------------------*/
int main()
{
	// system("color 03"); //mudar cor da console
	CAMPO_MINADO campoMinado;
	Boolean perdeu = FALSE, ganhou = FALSE;
	int m, n, z;
    
	printf("\n\n\t\t--------- Numero de Linhas ----------\n");	m = lerNum(3, MMAX);
	printf("\n\n\t\t--------- Numero de Colunas ---------\n");	n = lerNum(3, NMAX);
	printf("\n\n\t\t--------- Numero de Minas ----------\n");	z = lerNum(1, m * n - 1); 
	printf("\n\n\t\t------- Semente da Partida --------\n");	srand(lerNum(1, SMAX));
	
	inicializarTabuleiro(&campoMinado,  m,  n, 0, 0);
	campoMinado.posMarcadas = 0, campoMinado.posAbertas = 0;
	
	telaDeInicio();

	distribuirMinas(&campoMinado,  m,  n, z);
	preencherCampo(&campoMinado, m, n, z, 0, 0);
	imprimirCampo(campoMinado , m, n, perdeu, ganhou);
	jogo(&campoMinado, m, n, z, &perdeu, &ganhou);
	
	printf("\n\n\n");
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
//											Definiçoes das Funcoes
/*--------------------------------------------------------------------------------------------------------------------------------
	Objectivo: 	Imprimir tela de inicio 		                               
----------------------------------------------------------------------------------------------------------------------------------*/
void telaDeInicio()
{
	system("clear"); //limpatela; 
	puts("\n\n\t-------------------------------------------------------------------------------");
	puts("\t      .--.                               .-..-. _                 .-.          "); 
	puts("\t     : .--'                              : `' ::_:                : :          "); 
	puts("\t     : :    .--.  ,-.,-.,-..---.  .--.   : .. :.-.,-.,-. .--.   .-' : .--.     "); 
	puts("\t     : :__ ' .; ; : ,. ,. :: .; `' .; :  : :; :: :: ,. :' .; ; ' .; :' .; :    "); 
	puts("\t     `.__.'`.__,_;:_;:_;:_;: ._.'`.__.'  :_;:_;:_;:_;:_;`.__,_;`.__.'`.__.'    "); 
	puts("\t                           : :                                                 ");      
	puts("\t                           :_;                                                 ");
	printf("\n\n\t-------------------------------------------------------------------------------");
	printf("\n\t---------------------|  CAMPO MINADO - Busca Minas v1.0  |---------------------");
	printf("\n\t---------------------|  Desenvolvido por EDSON GREGORIO  |---------------------");
	printf("\n\t-------------------------------------------------------------------------------");
	printf("\n\n\t-----------------------A(abrir) - M(marcar) - D(desmarcar)---------------------\n");
}
/*-----------------------------------------------------------------------------------------------------------------------
	Objectivo: Ler um numero detro de um limite 		Retorna: Um numero valido                                
------------------------------------------------------------------------------------------------------------------------*/
int lerNum(int min, int max)
{
	int num;
	printf("\n\t\t>> Informe um numero entre %d e %d: ", min, max); scanf("%d", &num);
	if(num >= min && num <= max) return num;
	else printf("\t\t\tERRO: Numero invalido! \n");
	return lerNum(min, max);
}
/*-------------------------------------------------------------------------------------------------------------------------------------------
	Objectivo:  inicializar com zeros o tabuleiro de jogo (matriz)		                               
--------------------------------------------------------------------------------------------------------------------------------------------*/
void inicializarTabuleiro(CAMPO_MINADO *campoMinado, int m, int n, int i, int j)
{
	if(i < m){
		if(j < n){
			campoMinado->tabuleiroAuxiliar[i][j] = 0;
			campoMinado->tabuleiroPrincipal[i][j] = 0;
			inicializarTabuleiro(campoMinado, m, n, i, j + 1);
		}
		else inicializarTabuleiro(campoMinado, m, n, i + 1, 0);
	}
}
/*------------------------------------------------------------------------------------------------------------------------
	Objectivo: imprimir o numero de colunas da matriz 	                              
-------------------------------------------------------------------------------------------------------------------------*/
void imprimirColunas(int i, int n)
{
	if(i <= n){
		printf(" %d ", i);
		imprimirColunas(i+1, n);
	}	
}
/*-------------------------------------------------------------------------------------------------------------------------
	Objectivo: Imprimir os elementos de cada linha da matriz (Aij)                                  
--------------------------------------------------------------------------------------------------------------------------*/
void imprimirElemento(CAMPO_MINADO campoMinado, int m, int n, int i, int j, Boolean perdeu, Boolean ganhou)
{
	if (i < m){
		if(j == n)  printf("|  %d", i+1);
		if (j < n){	
			if(j == 0)  printf("  %d  |", i+1);
			if(perdeu == TRUE)
				(campoMinado.tabuleiroPrincipal[i][j] == MINA) ? printf(" @ ") :  printf(" %d ", campoMinado.tabuleiroPrincipal[i][j]);	
			else 
				if(ganhou == TRUE)
					(campoMinado.tabuleiroPrincipal[i][j] == MINA) ? printf(" * ") :  printf(" %d ", campoMinado.tabuleiroPrincipal[i][j]);	
				else 
					if(campoMinado.tabuleiroAuxiliar[i][j] == ABERTA)  printf(" %d ", campoMinado.tabuleiroPrincipal[i][j]);
					else  
						if(campoMinado.tabuleiroAuxiliar[i][j] == MARCADA)  printf(" * "); 
						else printf(" . "); 
			imprimirElemento(campoMinado, m, n, i, j + 1, perdeu, ganhou);
		}
		else{
			printf("\n\t\t\t\t");	
			imprimirElemento(campoMinado, m, n, i + 1 , 0, perdeu, ganhou);
		}	
	}	
}
/*-----------------------------------------------------------------------------------------------------------------------------
	Objectivo: Combina as impre�oes  		                               
-------------------------------------------------------------------------------------------------------------------------------*/
void imprimirCampo(CAMPO_MINADO campoMinado, int m, int n, Boolean perdeu, Boolean ganhou)
{
	telaDeInicio();
	printf("\n\t\t\t\t     ");
	imprimirColunas(1, n);
	printf("\n\n\t\t\t\t");
	imprimirElemento(campoMinado, m, n, 0 , 0, perdeu, ganhou);
	printf("\n\t\t\t\t     ");
	imprimirColunas(1, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------
	Objectivo: Distribuir as minas pelo tabuleiro aleatoriamente  	Recebe: m -> n de linhas, n-> n de colunas, z -> n de minas	                              
------------------------------------------------------------------------------------------------------------------------------------*/
void distribuirMinas(CAMPO_MINADO *campoMinado, int m, int n, int z)
{	
	int i, j, numAleatorio = rand();
	if(z > 0)
	{
		i = numAleatorio % m;
		j = (numAleatorio / m) % n;
		if(campoMinado->tabuleiroPrincipal[i][j] == MINA)	distribuirMinas(campoMinado, m, n, z);
		else {
			campoMinado->tabuleiroPrincipal[i][j] = MINA;
			distribuirMinas(campoMinado, m, n, --z);	
		}
	}	
}
/*--------------------------------------------------------------------------------------------------------------------------------------
	Objectivo:  Contar o numero de minas adjacentes a uma determinada posicao sem mina		Retorna: o numero de minas em volta dela                               
----------------------------------------------------------------------------------------------------------------------------------------*/
int minasAdjacentes(CAMPO_MINADO *campoMinado,int m, int n, int i, int j, int deltaL[], int deltaC[], int numMinas, int k)
{
	if(k < 0)	return numMinas;
  int linha = i + deltaL[k], coluna = j + deltaC[k];     
	if( linha >= 0 && coluna >= 0 && linha < m && coluna < n && campoMinado->tabuleiroPrincipal[linha][coluna] == MINA)	
			numMinas++;
	return minasAdjacentes(campoMinado, m, n, i, j, deltaL, deltaC, numMinas, --k);
}
/*---------------------------------------------------------------------------------------------------------------------------------------
	Objectivo: Preencer o tabuleiro com numeros equivalentes ao numero de minas adjacentes de determinadas posicoes da matriz                                
-----------------------------------------------------------------------------------------------------------------------------------------*/
void preencherCampo(CAMPO_MINADO *campoMinado, int m, int n, int z, int i, int j)
{	
	if (i < m){
		if (j < n){
			if(campoMinado->tabuleiroPrincipal[i][j] != MINA){
			 	int deltaL[] = {-1, 1, 1, -1, 1, -1, 0, 0}, deltaC[] = {-1, 1, -1, 1, 0, 0, 1, -1};
			 	campoMinado->tabuleiroPrincipal[i][j] = minasAdjacentes(campoMinado, m, n, i, j, deltaL, deltaC, 0, 7);
			}
			preencherCampo(campoMinado, m, n, z, i, j + 1);
		}
		else preencherCampo(campoMinado, m, n, z, i + 1, 0);
	}
}
/*---------------------------------------------------------------------------------------------------------------------------------------
	Objectivo: Abrir posi��es adjacentes � posic�o vazia  		Recebe: Dois vetores com possiveis deslocamentos                                
-----------------------------------------------------------------------------------------------------------------------------------------*/
void abrirAdjacentes(CAMPO_MINADO *campoMinado, int i, int j, int m, int n, int deltaL[], int deltaC[], int k)
{
	if(k >= 0){
		int linha = i + deltaL[k], coluna = j + deltaC[k];
		if(campoMinado->tabuleiroPrincipal[linha][coluna] != MINA && campoMinado->tabuleiroAuxiliar[linha][coluna] != ABERTA) 
		{
			campoMinado->tabuleiroAuxiliar[linha][coluna] = ABERTA;	
			if(linha >= 0 && coluna >= 0 && linha < m && coluna < n)
			{
				campoMinado->posAbertas++;
			    if(campoMinado->tabuleiroPrincipal[linha][coluna] == 0)	
					abrirAdjacentes(campoMinado, linha, coluna, m, n, deltaL, deltaC, 7);	
			}					
		}
		abrirAdjacentes(campoMinado, i, j, m, n, deltaL, deltaC, --k);	
	}
}
/*-------------------------------------------------------------------------------------------------------------------------------------------
	Objectivo: abrir determinada posisao da matriz  (perde se abrir uma posicao com mina ou abre o Adjacentes de uma de terminada pos. vazia)                              
--------------------------------------------------------------------------------------------------------------------------------------------*/
int abrirPosicao(CAMPO_MINADO *campoMinado, int i, int j, int m, int n, int z, Boolean *perdeu, Boolean *ganhou)
{
	if(campoMinado->tabuleiroPrincipal[i][j] == MINA){
		*perdeu = TRUE;
		return PERDEU;
	} 	
	else {
		campoMinado->tabuleiroAuxiliar[i][j] = ABERTA;
		campoMinado->posAbertas++;
		if(campoMinado->tabuleiroPrincipal[i][j] == 0) 
		{
			int deltaL[] = {-1, 1, 1, -1, 1, -1, 0, 0}, deltaC[] = {-1, 1, -1, 1, 0, 0, 1, -1};
			abrirAdjacentes(campoMinado, i, j, m, n, deltaL, deltaC, 7);			
		}		
		if(campoMinado->posAbertas == (m*n-z)) 	return GANHOU;
		return CONTINUA;
	}
}
/*-------------------------------------------------------------------------------------------------------------------------------------------
	Objectivo: 	Ler a jogada do usuario 		Retorna: Um sem efeito caso a jogada seja invalida, ou continua...                                
--------------------------------------------------------------------------------------------------------------------------------------------*/
int lerJogada(CAMPO_MINADO *campoMinado, int m, int n,  int z, Boolean *perdeu, Boolean *ganhou)
{
	char operacao;
	int i = 0, j = 0;
	printf("\n\n\t\t\tProximo chute [letra] [linha] [coluna]: ");
	scanf("%c %d %d", &operacao, &i, &j);
	--i, --j;
	if(i < 0 || i >= m || j < 0 || j >= n || campoMinado->tabuleiroAuxiliar[i][j] == ABERTA) return SEM_EFEITO;
	if(operacao == 'A'  || operacao == 'a') return abrirPosicao(campoMinado, i, j, m, n, z, perdeu, ganhou);
	if(operacao == 'M'  || operacao == 'm'){
		if(campoMinado->posMarcadas == z)	return SEM_EFEITO;
		campoMinado->tabuleiroAuxiliar[i][j] = MARCADA;
		campoMinado->posMarcadas++;
	}
	else if(operacao == 'D'  || operacao == 'd'){
					if(campoMinado->tabuleiroAuxiliar[i][j] != MARCADA) return SEM_EFEITO;
					else campoMinado->tabuleiroAuxiliar[i][j] = DESMARCADA;	
					campoMinado->posMarcadas--;	
		}
		else return SEM_EFEITO;
	return CONTINUA;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------
	Objectivo:  chama a opcao de ler jogada recebe o seu valor de retorno e procede com base nele		                               
--------------------------------------------------------------------------------------------------------------------------------------------*/
void jogo(CAMPO_MINADO *campoMinado, int m, int n,  int z, Boolean *perdeu, Boolean *ganhou)
{
	int valRetorno = lerJogada(campoMinado, m, n, z, perdeu, ganhou);
	if(valRetorno == CONTINUA){
		imprimirCampo(*campoMinado, m, n, *perdeu, *ganhou);
		if(campoMinado->posAbertas == (m*n-z)) *ganhou = TRUE;
		printf("\n\n\t\t\tPor enquanto: %d/%d marcadas, %d livres.\n", campoMinado->posMarcadas, z, m*n - campoMinado->posAbertas - campoMinado->posMarcadas);
	}		
	else if(valRetorno == PERDEU){
			*perdeu = TRUE;
			imprimirCampo(*campoMinado, m, n, *perdeu, *ganhou);
			printf("\n\n\t\t\t\tBOOOOM! Pisou numa mina.\n\t\t\t\tGAME OVER: Voce perdeu!");	
		}
		else if(valRetorno == GANHOU || *ganhou == TRUE){
				*ganhou = TRUE;
				imprimirCampo(*campoMinado, m, n, *perdeu, *ganhou);
	 			printf("\n\t\t\t\tVoce encontrou todas as minas. \n\t\t\t\tParabens!\n");
	 		}
	 		else if(valRetorno == SEM_EFEITO)	printf("\n\t\t\t\tSem efeito!\n");
	 			
   	if(*perdeu != TRUE && *ganhou != TRUE) jogo(campoMinado, m, n, z, perdeu, ganhou);
}