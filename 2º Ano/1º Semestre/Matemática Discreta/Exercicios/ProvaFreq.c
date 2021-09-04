#include <stdio.h>
#include <stdlib.h>

/*********************PROT�TIPO DE FUN��ES *********************/
void imprimirVet(int vet[], int nElem);
void imprimirStr(char str[]);
char toUpper(char letra);
char toLower(char letra);
int verifConsoante(char letra);
int verifVogal(char letra);
void substituir(char str[], int i);
void MAX_MIN(int vet[], int nElem, int *MAX, int *MIN);
void organizar(int vet[], int nElem);
int EhCapicua(int vet[], int i, int f);
//****************************************************************

//imprimir numero por numero de um vetor
void imprimirVet(int vet[], int nElem)
{
	for(int i = 0; i < nElem; ++i) printf("|%d| ",vet[i]);
}

//imprimir caracter por caracter da cadeia
void imprimirStr(char str[])
{
	for(int i = 0; str[i] != '\0'; i++) printf("%c",str[i]);
}

//converter letra para maiusculo
char toUpper(char letra)
{
  return (letra >= 'a' && letra <= 'z') ? letra + ('A' - 'a') : letra;
}

//converter letra para minusculo
char toLower(char letra)
{
	return (letra >= 'A' && letra <= 'Z') ? letra - ('A' - 'a') : letra;
}

//verificar de uma letra � uma consoante
int verifConsoante(char letra)
{
	return (letra != 'a' && letra != 'e' && letra != 'i' && letra != 'o' && letra != 'u' 
		&& letra != 'A' && letra != 'E' && letra != 'I' && letra != 'O' && letra != 'U' 
		&& letra != ' ');
}

//verificar de uma letra � uma vogal
int verifVogal(char letra)
{
	return (letra == 'a'|| letra == 'e' || letra == 'i' || letra == 'o' || letra == 'u');
}

//EXERCICIO 3
//converter as vogais para Maiusculas e consoantes para minusculas
void substituir(char str[], int i)
{
	if(str[i]) {
		if(verifVogal(str[i])) str[i] = toUpper(str[i]);
		if(verifConsoante(str[i])) str[i] = toLower(str[i]);
		substituir(str, i+1);
	}	
}

//EXERCICIO 5 : devolver o maior valor e menor valor de um vetor
void MAX_MIN(int vet[], int nElem, int *MAX, int *MIN)
{
	nElem--;
	if(nElem >= 0) {
		if(vet[nElem] > *MAX) *MAX = vet[nElem];
		if(vet[nElem] < *MIN) *MIN = vet[nElem];
		MAX_MIN(vet, nElem, MAX, MIN);
	}
}

//EXERCICIO 1 : numeros pares no inicio e impares no fim
void organizar(int vet[], int nElem)
{
	int aux, i, j;
	for( i = 0; i < nElem; ++i)
		if((vet[i]%2) == 0)
			for(j = 0; j < i; ++j)
				if((vet[j]%2) != 0){
					aux = vet[j];
					vet[j] = vet[i];
					vet[i] = aux;
				}
}

//EXERCICIO 4
int EhCapicua(int vet[], int i, int f)
{
	int valor1, valor2, k;
	if(i == f) return 1;
  if(vet[i] != vet[f]) return 0;
  k = (i + f)/2;
  valor1 = EhCapicua(vet, i, k);
  valor2 = EhCapicua(vet, k+1, f);
}

int main()
{
	char str[] = {'e','D','S','o','N', ' ','P','a','u','L','O'};
	int vet[] = {2,5,4,11,5,8,7,51,20,15,12,19,10,14,21,9};
	int vetCapicua[] = {5,3,1,3,5};
	int nElem = 16;
	int MAX = vet[nElem-1], MIN = vet[nElem-1];

	imprimirVet(vet, nElem);
	printf("\n");
	organizar(vet, nElem);
	imprimirVet(vet, nElem);
	
	printf("\n\n");
	MAX_MIN(vet, nElem, &MAX , &MIN);
	printf("\nMAX: %d - MIN: %d", MAX, MIN);
	printf("\n\n");
	
	imprimirStr(str);
	printf("\n");
	substituir(str, 0);
	imprimirStr(str);
	
	printf("\n\n");
	if(EhCapicua(vetCapicua, 0, 4))
		printf("\nEh Capicua");
	else printf("\nNao Eh Capicua");

	printf("\n\n");
	system("pause");
	return 0;	
}
