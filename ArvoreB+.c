#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0
#define invalid -1
#define t 3

typedef struct aux{
	int n;
	int *chaves;
	struct aux ** filhos;
	int leaf;	
}NO;

typedef NO* PONT;

typedef PONT* TREE;

//cria uma nova árvoreB
PONT criaArvore(){
	PONT arvore = (PONT)malloc(sizeof(NO));
	arvore->chaves = (int*)malloc(sizeof(int)*2*t-1);
	arvore->filhos = (PONT*)malloc(sizeof(PONT)*2*t);
	arvore->n = 0;
	arvore->leaf = true;
	return arvore;
}

//busca para encontrar posição onde deve ser inserido/removido um item
PONT busca(PONT pai, int chave, int* retorno){
	*retorno = -1;
	int i =0;
	while (chave > pai->chaves[i] && i < pai->n) i++;
	*retorno = i;
	if (chave == pai->chaves[i]) return pai;
	if(!(pai->leaf)) return busca(pai->filhos[i], chave, retorno);
	return NULL;
}

/////////INSERÇÃO/////////

//funções auxiliares para a inserção
void moveParaDireita(PONT pai, int pos){
	int i;
	for (i = pai->n-1; i > pos-1; i--){
		pai->chaves[i+1] = pai->chaves[i];
		pai->filhos[i+2] = pai->filhos[i+1];
	}
	pai->n++;
}

void divideNoFilho(PONT pai, int pos){
	PONT filho = pai->filhos[pos];
	PONT irmao = (PONT)malloc(sizeof(NO));
	irmao->n = t-1;
	irmao->leaf = filho->leaf;
	irmao->chaves = (int*)malloc(sizeof(int)*2*t-1);
	irmao->filhos = (PONT*)malloc(sizeof(PONT)*2*t);
	int i;
	for (i = 0; i < t-1; i++){
		irmao->chaves[i] = filho->chaves[t+i];
		irmao->filhos[i] = filho->filhos[t+i];
	}
	irmao->filhos[i] = filho->filhos[2*t-1];
	filho->n = t-1;
	moveParaDireita(pai, pos);
	pai->chaves[pos] = filho->chaves[t-1];
	pai->filhos[pos+1] = irmao;
}
// fim das funções auxiliares

//inserção
int insere(TREE arvore,PONT pai, int chave){
	if ((*arvore)->n == 2*t-1){
		PONT novaRaiz = criaArvore();
		novaRaiz->filhos[0] = *arvore;
		divideNoFilho(novaRaiz, 0);
		novaRaiz->leaf = false;
		*arvore = novaRaiz;
		return insere(arvore, novaRaiz, chave);
	}
	else if(pai->leaf){
		int i = 0;
		while(chave > pai->chaves[i] && i < pai->n) i++;
		if(chave == pai->chaves[i]) return false;
		moveParaDireita(pai, i);
		pai->chaves[i] = chave;
		return true;
	}
	else{
	
		int i = 0;
		while(chave > pai->chaves[i] && i < pai->n) i++;
		if(pai->filhos[i]->n == 2*t-1) {
			divideNoFilho(pai, i);
			int j;
			if(pai->chaves[i] > chave) return insere(arvore, pai->filhos[i], chave);
			else if(pai->chaves[i] < chave){
				return insere(arvore, pai->filhos[i+1], chave);
			}
			else return false;
		}
		if(pai->chaves[i] == chave){
			return false;
		}
		else insere(arvore, pai->filhos[i], chave);
	}
}

/////////REMOÇÃO/////////

//funções auxiliares para a remoção
void moveParaEsquerda (PONT pai, int pos){
	int i;
	for(i = pos; i < pai->n-1; i++){
		pai->chaves[i] = pai->chaves[i+1];
		pai->filhos[i+1] = pai->filhos[i+2];
	}
	pai->n--;
}
PONT antecessor (PONT pai, int * retorno){
	//printf("%i\n",pai->n);
	if (!(pai->leaf)) return antecessor(pai->filhos[pai->n], retorno);
	*retorno = pai->chaves[pai->n-1];
	return pai;
}
PONT sucessor (PONT pai, int * retorno){
	if (!(pai->leaf)) return antecessor(pai->filhos[0], retorno);
	*retorno = pai->chaves[0];
	return pai;
}
void juntaNo(TREE raiz, PONT pai, int pos, int Irmao, int* flag){
	int i;
	*flag = 0;
	PONT filho = pai->filhos[pos];
	PONT irmao = pai->filhos[Irmao];
	if(Irmao > pos){
		filho->n = 2*t - 1;
		filho->chaves[t-1] = pai->chaves[pos];
		for(i = 0; i < irmao->n; i++){
			filho->chaves[i+t] = irmao->chaves[i];
			filho->filhos[i+t] = irmao->filhos[i];
		}
		filho->filhos[filho->n] = irmao->filhos[i];
	}
	else{
		moveParaDireita(filho,0);
		filho->chaves[0] = pai->chaves[pos];
		for(i = 0; i < irmao->n; i++){
			moveParaDireita(filho,0);
			filho->chaves[0] = irmao->chaves[irmao->n-1 -i];
			filho->filhos[0] = irmao->filhos[irmao->n-1-i];
		}
		filho->filhos[0] = irmao->filhos[0];
	}
	moveParaEsquerda(pai, pos);
	if(pai == *raiz && (*raiz)->n == 0){
		*flag = 1;
		*raiz = filho;
	}
	int k;
	free(irmao);
}
//fim das funções auxiliares

//remoção
int remocao(TREE raiz, PONT pai, int chave){
	int i = 0;
	while(pai->chaves[i] < chave && i < pai->n) i++;
	if(!(pai->leaf)){
		if(pai->chaves[i] == chave){
			int * retornoA = (int*)malloc(sizeof(int));
			int * retornoS = (int*)malloc(sizeof(int));
			PONT ant = antecessor(pai->filhos[i], retornoA);
			PONT suc = sucessor(pai->filhos[i+1],retornoS);
			if(pai->filhos[i]->n > t-1){
				pai->chaves[i] = *retornoA;
				return remocao(raiz, pai->filhos[i], *retornoA);
			}
			else if(i < pai->n && pai->filhos[i+1]->n > t-1){
				pai->chaves[i] = *retornoS;
				return remocao(raiz, pai->filhos[i+1], *retornoS);
			}
			else {
				int* flag = (int*)malloc(sizeof(int));
				if(i < pai->n) juntaNo(raiz, pai,i,i+1, flag);
				else juntaNo(raiz, pai,i,i-1, flag);
				return remocao(raiz, pai->filhos[i], chave);
			}
		}
		else {
			int j = 0;
			PONT filho = pai->filhos[i];
			while (filho->chaves[j] < chave && j < filho->n) j++;
				if(filho->n == t-1){
					if(i < pai->n && pai->filhos[i+1]->n > t-1){
						PONT irmaoD = pai->filhos[i+1];
						filho->n++;
						filho->chaves[t-1] = pai->chaves[i];
						filho->filhos[t] = irmaoD->filhos[0];
						pai->chaves[i] = irmaoD->chaves[i];
						moveParaEsquerda(irmaoD, 0);
						return remocao(raiz, filho, chave);
					}
					else if(i > 0 && pai->filhos[i-1]->n > t-1){
						PONT irmaoE = pai->filhos[i-1];
						filho->n++;
						filho->chaves[t-1] = pai->chaves[i];
						filho->filhos[t] = irmaoE->filhos[0];
						pai->chaves[i] = irmaoE->chaves[i];
						moveParaEsquerda(irmaoE, 0);
						return remocao(raiz, filho, chave);
					}
					else{
						int* flag = (int*)malloc(sizeof(int));
						if(i < pai->n){
						 juntaNo(raiz,pai,i,i+1,flag);
						 if(*flag) free(pai);
						 return remocao(raiz,filho,chave);
						// if(*flag) free(pai);
						}
						else {
							filho = pai->filhos[i-1];
							juntaNo(raiz,pai,i-1,i,flag);
							if(*flag) free(pai);
							return remocao(raiz,filho,chave);
							//if(*flag) free(pai);
						} 
					}
				}
				else return remocao(raiz,filho,chave);
		}
	}
	else{
		if (pai->chaves[i] == chave){
			moveParaEsquerda(pai,i);
			return true;
		}
		return false;
	}
}

/////////IMPRESSÃO/////////
void imprime(PONT pai, FILE* arqui){
	int i;
	fprintf(arqui,"(");
	for (i = 0; i<pai->n+1; i++){
		if(!(pai->leaf)) imprime(pai->filhos[i],arqui);
		if(i < pai->n)fprintf(arqui,"%i, ",pai->chaves[i]);
	}
	fprintf(arqui,")");
}
int main(int argc, char *argv[]) {
	
	FILE *arqui = NULL;
	FILE *arquiW = NULL;
	
	arqui = fopen(argv[1], "r");
	arquiW = fopen(argv[2], "w");
	char operacao[32];
	
	TREE raiz = (TREE)malloc(sizeof(PONT));
	*raiz = criaArvore();

	while(fscanf(arqui,"%s", operacao) == 1 && strcmp(operacao,"fim")){
		if(strcmp(operacao, "insere") == 0){
			int chave;
			fscanf(arqui,"%i", &chave);
			insere(raiz, *raiz,chave);
		}
		else if(strcmp(operacao, "remove") == 0){
			int chave;
			fscanf(arqui,"%i", &chave);
			remocao(raiz, *raiz,chave);
		}
		else if (strcmp(operacao,"imprime") == 0) {
			imprime(*raiz,arquiW);
		}
	}
	fclose(arquiW);
	return 0;
}
