//Nome: César Billalya Yamasato
//número usp: 12542992

#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0
#define infinite 99999

typedef struct aux{
	float peso;
	int vdest;
	struct aux* prox;
}ARESTA;

typedef struct{
	float custo;
	int id;
	int posicao;
	ARESTA* listaadj;
}VERTICE;

typedef ARESTA* PONT;
typedef VERTICE* PONTV;

typedef struct{
	int numArestas;
	int numvertices;
	PONT* vertices;
	int direcionado;
}GRAFO;

typedef struct{
	int* referencias;
	int numElementos;
	int numElementosInit;
	PONTV* referenciaV;
	PONTV* fila;
}HEAP;

typedef struct{
	int* ant;
	float custo;
	int* arvore;
}ARVORE;

GRAFO* InicializaGrafoVazio(int numv,int numa, int direcionado){
	int i;
	GRAFO* grafo = (GRAFO*) malloc(sizeof(GRAFO));
	grafo->	numArestas = numa;
	grafo->	numvertices = numv;
	grafo->	vertices = (PONT*) malloc(sizeof(PONT)*numv);
	grafo->direcionado = direcionado;
	
	for (i = 0; i < numv; i++){
		grafo->vertices[i] = (PONT)malloc(sizeof(ARESTA));
		grafo->vertices[i]->peso=-1;
		grafo->vertices[i]->prox=NULL;
		grafo->vertices[i]->vdest=-1;
		
	}
	return grafo;
}

int InsereAresta(GRAFO* grafo, int vertice1, int vertice2, float peso){
	if (vertice1 >= 0 && vertice1 < grafo->numvertices)
	{
		if (vertice2 >= 0 && vertice2 < grafo->numvertices)
		{
			PONT aresta = (PONT)malloc(sizeof(ARESTA));
			aresta->peso = peso;
			aresta->prox = NULL;
			aresta->vdest = vertice2;
			PONT inicio = grafo->vertices[vertice1];
			PONT atual = inicio;
			while (inicio != NULL){
				if(inicio->vdest == vertice2){
					free(aresta);
					return false;
				}
				atual = inicio;
				inicio = inicio->prox;
			}
			atual->prox = aresta;
			if(!grafo->direcionado) InsereAresta(grafo, vertice2, vertice1, peso);
			return true;
		}
	}
	return false;
}

int ExisteAresta(GRAFO* grafo, int vertice1, int vertice2){
	if (vertice1 >= 0 && vertice1 < grafo->numvertices)
	{
		if (vertice2 >= 0 && vertice2 < grafo->numvertices)
		{
			PONT inicio = grafo->vertices[vertice1];
			while (inicio != NULL){
				if(inicio->vdest == vertice2){
					return true;
				}
				inicio = inicio->prox;
			}
			
		}
	}
	return false;
}

PONT VerticeAdj(GRAFO* grafo, int vertice){
	if (grafo->vertices[vertice]->prox == NULL) return NULL;
	else return grafo->vertices[vertice]->prox;
}

int RetiraAresta(GRAFO* grafo, int vertice1, int vertice2){
	if (vertice1 >= 0 && vertice1 < grafo->numvertices)
	{
		if (vertice2 >= 0 && vertice2 < grafo->numvertices)
		{
			PONT inicio = grafo->vertices[vertice1];
			PONT atual = inicio;
			while (inicio != NULL){
				if(inicio->vdest != -1 && inicio->vdest == vertice2){
					atual->prox = inicio->prox;
    				RetiraAresta(grafo,vertice2,vertice1);
					free(inicio);
					if(!grafo->direcionado) RetiraAresta(grafo, vertice2, vertice1);
					return true;
				}
				atual = inicio;
				inicio = inicio->prox;
				
			}
		}
	}
	return false;
}

void LiberaGrafo(GRAFO* grafo){
	int i;
	for (i = 0; i<grafo->numvertices;i++){
		PONT inicio = grafo->vertices[i];
		PONT atual = inicio;
		while(inicio != NULL){
			atual = inicio;
			inicio = inicio->prox;
			free(atual);
		}	
	}
	free(grafo->vertices);
	free(grafo);
}

void ImprimeGrafo(GRAFO* grafo){
	float grafoM[grafo->numvertices][grafo->numvertices];
	
	int i;
	int j;
	
	for(i = 0; i<grafo->numvertices; i++){
		for(j = 0; j<grafo->numvertices; j++){
			grafoM[i][j] = 0;
		}
		PONT inicio = grafo->vertices[i];
		while(inicio != NULL){
			if (inicio->vdest != -1) grafoM[i][inicio->vdest] = inicio->peso;
			inicio = inicio->prox;
		}
	}
	printf(" ");
	for(i = 0; i<grafo->numvertices; i++) printf(" %g", i);
	printf("\n");
	for(i = 0; i<grafo->numvertices; i++){
		printf("%g", i);
		for(j = 0; j<grafo->numvertices; j++){
			printf(" %g", grafoM[i][j]);
		}
		printf("\n");
	}
}

void GrafoTransposto(GRAFO* grafo){
	float grafoM[grafo->numvertices][grafo->numvertices];
	int i;
	int j;
	
	for(i = 0; i<grafo->numvertices; i++){
		for(j = 0; j<grafo->numvertices; j++){
			grafoM[i][j] = 0;
		}
	}
	for(i=0; i<grafo->numvertices; i++){
		PONT inicio = grafo->vertices[i];
		PONT atual = inicio;
		while(inicio != NULL){
			atual = inicio;
			inicio = inicio->prox;
			if (atual->vdest != -1){
				if(grafoM[i][atual->vdest] == 0){
					InsereAresta(grafo, atual->vdest, i, atual->peso);
					grafoM[atual->vdest][i] = 1;
					RetiraAresta(grafo, i, atual->vdest);
				}
			}
		}
	}
}

///////// HEAP ///////////

void FazHeapMaximo(HEAP* heap, int raiz){
	if(raiz >= 0 && raiz < heap->numElementos){
		int l = raiz*2 + 1;
		int r = raiz*2 + 2;
		int maior = raiz;
		VERTICE* aux;
		int auxiliar;
		
		if (heap->numElementos > l && heap->fila[l]->custo < heap->fila[maior]->custo) maior = l;
		if (heap->numElementos > r && heap->fila[r]->custo < heap->fila[maior]->custo) maior = r;
		
		if(maior != raiz){
			aux = heap->fila[raiz];
		
			heap->fila[maior]->posicao = raiz;
			heap->fila[raiz]->posicao = maior;

			heap->fila[raiz] = heap->fila[maior];
			heap->fila[maior] = aux;
			//printf("teste:%i : %i\n", maior, heap->fila[maior]->custo);
			FazHeapMaximo(heap, maior);
		}
	}
}

HEAP* FazHeap(GRAFO* grafo){
	int i;

	HEAP* heap = (HEAP*)malloc(sizeof(HEAP));
	heap->fila = (PONTV*)malloc(sizeof(PONTV)*grafo->numvertices);
	heap->referenciaV = (PONTV*)malloc(sizeof(PONTV)*grafo->numvertices);
	heap->referencias = (int*)malloc(sizeof(int)*grafo->numvertices);
	heap->numElementos = grafo->numvertices;
	heap->numElementosInit = grafo->numvertices;
	
	for (i = 0; i < grafo->numvertices; i++){
		heap->fila[i] = (PONTV)malloc(sizeof(VERTICE));
		heap->referenciaV[i] = (PONTV)malloc(sizeof(VERTICE));
		heap->fila[i]->custo = infinite;
		heap->fila[i]->posicao = i;
		heap->referencias[i] = 1;
		heap->fila[i]->id = i;
		heap->fila[i]->listaadj = NULL;
		if(grafo->vertices[i]->prox) heap->fila[i]->listaadj = grafo->vertices[i]->prox;
		heap->referenciaV[i] = heap->fila[i];
	}
	return heap;
}

VERTICE* RetiraMin(HEAP* heap){
	if(heap->numElementos){
		VERTICE* res = heap->fila[0];
		heap->fila[0] = heap->fila[heap->numElementos-1];
		heap->referenciaV[0]->posicao = 0;
		heap->referencias[res->id] = 0;    
		//heap->fila = (VERTICE**)realloc(heap->fila, sizeof(VERTICE)*heap->numElementos-1);
		heap->numElementos--;
		//printf("RetiraMin: %i\n",heap->fila[0]->id);
		FazHeapMaximo(heap, 0);
		return res;
	}
	return NULL;
}

int MudaPrioridade(HEAP* heap, int vertice, float novaPrio){
	if(vertice >= 0 && vertice < heap->numElementosInit){
		if(heap->referenciaV[vertice]->custo > novaPrio){
			heap->referenciaV[vertice]->custo = novaPrio;
			int posicao = heap->referenciaV[vertice]->posicao;
			int pai = (posicao-1)/2;
			while(heap->fila[posicao]->custo < heap->fila[pai]->custo){
				VERTICE* auxiliar = heap->fila[pai];
				heap->fila[pai] = heap->fila[posicao];
				heap->fila[pai]->posicao = pai;
				heap->fila[posicao] = auxiliar;
				heap->fila[posicao]->posicao = posicao;
				
				posicao = pai;
				pai = (posicao-1)/2;
			}
			FazHeapMaximo(heap,posicao);
			return true;
		}
	}
	return false;
}

///////// PRIM ///////////

ARVORE* Prim(GRAFO* grafo, int raiz){
	int i;
	HEAP* heap = FazHeap(grafo);
	ARVORE* arvore = (ARVORE*)malloc(sizeof(ARVORE));
	arvore->ant = (int*)malloc(sizeof(int)*grafo->numvertices);
	arvore->arvore = (int*)malloc(sizeof(int)*grafo->numvertices);
	arvore->custo = 0;
	
	for (i = 0; i <heap->numElementos; i++)arvore->ant[i] = -1;
	MudaPrioridade(heap, raiz, 0);
	VERTICE* atual;
	PONT aux;
	i = 0;
	int j;
	while(heap->numElementos){
		//for(j = 0; j<grafo->numvertices; j++) printf("id:%i, ",heap->fila[j]->id);
		//printf("\n");
		atual = RetiraMin(heap);
		arvore->custo += atual->custo;
		arvore->arvore[i] = atual->id;
		i++;
		aux = atual->listaadj;
		while(aux != NULL){
			if(aux->peso < heap->referenciaV[aux->vdest]->custo && heap->referencias[aux->vdest]){
				//printf("atual: %i vdest:%i \n", atual->id, aux->vdest);
				arvore->ant[aux->vdest] = atual->id;
				MudaPrioridade(heap, aux->vdest,aux->peso);
			}
			aux = aux->prox;
		}
	}
	return arvore;
}

FILE *arqui = NULL;
FILE *arquiW = NULL;

int main(int argc, char const *argv[]){
	//encontrando os arquivos de entrada e saída
	arqui = fopen(argv[1], "r");
	arquiW = fopen(argv[2], "w");
	
	//lendo o arquivo de entrada para gerar o grafo
	int vertices;
	int arestas;
	fscanf(arqui, "%i", &vertices);
	fscanf(arqui, "%i", &arestas);
	GRAFO* grafo = InicializaGrafoVazio(vertices,arestas, 0);
	
	//Inserindo as arestas no grafo
	int i;
	int vertice1;
	int vertice2;
	float peso;

	for(i = 0; i < arestas; i++){
		fscanf(arqui, "%i", &vertice1);
		fscanf(arqui, "%i", &vertice2);
		fscanf(arqui, "%g", &peso);
		InsereAresta(grafo,vertice1,vertice2,peso);
	}
	
	//Algoritmo de Prim
	ARVORE* arvore = Prim(grafo, 0);
	
	//Imprimindo o resultado no arquivo de saída
	fprintf(arquiW,"%g\n", arvore->custo);
	for(i = 1; i<grafo->numvertices; i++) fprintf(arquiW,"%i %i\n",arvore->ant[arvore->arvore[i]], arvore->arvore[i]);
	
	return 0;
}
    
