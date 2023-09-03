#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define ARQUIVO "dimacs.txt"
#define ST 1
// Feito por: Jo�o Vitor Neves Costa

typedef struct Elemento{ //elemento usado na lista de adjac�ncia
	int aresta;
	int peso;
	struct Elemento *prox;
}elemento;

typedef struct{ //struct para lista de adjac�ncia
	char cor;
	int p,d,f;
	elemento *inicio;
}lista;

typedef struct{ //fila para busca em largura
	elemento *inicio;
	elemento *final;
}fila;

typedef struct{ //lista est�tica/dinamica para conjunto Q do algor�tmo Dijkstra
	int *x;
	int fim;
}conjunto;

//Lista de Adjac�ncia
lista* criar_lista(int v);
elemento* inserir(elemento *r,int y, int z); //insere recursivo
void inserir_lista(lista *l, int n, int x, int y, int z); //insere adjac�ncia
void imprimir(lista *l, int n); //imprime o Grafo como Lista de Adjac�ncia
lista* lista_adj(int *n); //le o arquivo DIMACS e cria uma Lista de Ajac�ncia com ele

//Busca em Profundidade
int visita(lista *l, int n, int x,int tempo); //fun��o recursiva da DFS
void busca_p(lista *l, int n, int s); //DFS com inicio no v�rtice s

//Busca em Largura
fila* criar_fila(); //fila de v�rtices
void inserir_fila(fila *f,int x); //insere v�rtice na lista
void remover_fila(fila *f,int *x); //remove v�rtice da lista
void busca_l(lista *l, int n, int s); //BFS com inicio no v�rtice s

//Algor�tmo Dijkstra
conjunto* criar_conj(int max); //cria conjunto Q
void inserir_conj(conjunto *c,int max, int x); //insere v�rtices no conjunto
void remover_conj(conjunto *c,int *x, int pos); //remove aresta segura do conjunto
int minimo(lista *l,int n,conjunto *c); //encontra a menor aresta do conjunto
void relax(lista *l, int n, int u, elemento *v); //verifica se dist�ncia do vertice atual � menor que dist�ncia do predecessor + peso da aresta
void Dijkstra(lista *l, int n,int s); //Algoritmo Dijkstra com inicio no v�rtice s

//Funcionalidades de Grafo
void encontrar_aresta(lista *l, int n, int *v, int *w, int a, int *p); //encontra aresta na lista por ordem do arquivo DIMACS
int aresta_adj(lista *l, int n, int a); //Imprime todas as arestas adjacentes a uma aresta "a"
void vertice_adj(lista *l, int n, int v); //Imprime todos os v�rtices adjacentes a um v�rtice "v"
void vertice_inc(lista *l, int n, int v); //Imprime todas as arestas incidentes a um v�rtice "v"
int aresta_inc(lista *l, int n, int a); //Imprime todos os v�rtices incidentes a uma aresta "a"
void grau(lista *l, int n, int v); //Calcula os graus de entrada e sa�da de um v�rtice "v"
int vertice_adj_2(lista *l, int n, int v, int w); //Verifica se dois v�rtices s�o adjacentes
int trocar_peso(lista *l, int n, int a, int p); //Troca o peso de uma aresta
int trocar_vertices(lista *l, int n, int v, int w); //Troca dois v�rtices de lugar

//Menu principal
void menu(lista *l, int *n); //menu onde o usu�rio interage

int main(){
	setlocale(LC_ALL,"Portuguese");
	int n;
	lista *l;
	l=lista_adj(&n); //cria lista com ARQUIVO
	
	menu(l,&n); //passa lista criada para o menu
	
	free(l);
	
	return 0;
}

lista* criar_lista(int v){
	int i;
	lista *l=(lista*)malloc(v*sizeof(lista)); //cria vetor de listas
	
	for(i=0;i<v;i++){ //define inicio nulo, cor branca e tempos de descoberta/dist�ncia e finaliza��o para 0
		l[i].inicio=NULL;
		l[i].cor='B';
		l[i].f=0;
		l[i].d=0;
	}
	
	return l;
}

elemento* inserir(elemento *r,int y, int z){
	if(r==NULL){
		elemento *novo=(elemento*)malloc(sizeof(elemento));
		novo->aresta=y;
		novo->peso=z;
		novo->prox=NULL;
		
		r=novo;
		
		return r;
	}
	else{
		r->prox=inserir(r->prox,y,z);
	}
	return r;
}

void inserir_lista(lista *l,int n, int x, int y, int z){ //trabalha com x-1 e recebe y como y-1
	if(l[x-1].inicio==NULL){ 
		elemento *novo=(elemento*)malloc(sizeof(elemento));
		novo->aresta=y;
		novo->peso=z;
		novo->prox=NULL;
		
		l[x-1].inicio=novo;
	}
	else{
		l[x-1].inicio=inserir(l[x-1].inicio,y,z);
	}
}

void imprimir(lista *l, int n){ //imprime em caracter ou numero, dependendo das configura��es do c�digo
	int i;
	elemento *aux;
	
	for(i=0;i<n;i++){
		aux=l[i].inicio;
		
		printf("V%d:\t",i+ST);
		while(aux!=NULL){
			if(aux->prox!=NULL){
				printf("V%d Peso=%d ->\t",aux->aresta+ST,aux->peso);
			}
			else{
				printf("V%d Peso=%d -//\t",aux->aresta+ST,aux->peso);
			}
			aux=aux->prox;
		}
		printf("\n");
	}
}

lista* lista_adj(int *n){
	FILE *arq=fopen(ARQUIVO,"r");
	int arest=-1,x=0,y=0,z=0,i=0;
	lista *l;
	*n=-1;
	
	if(arq!=NULL){
		printf("Arquivo aberto com sucesso!\n");
	}
	else{
		printf("Falha em abrir o arquivo!\n");
		exit(1);
	}
	
	fscanf(arq,"%d %d\n",n,&arest);	//usa a primeira linha do padr�o dimacs
	
	l=criar_lista(*n);	//cria lista com inicio NULL
	
	do{
		fscanf(arq,"%d %d %d\n",&x,&y,&z);
		
		inserir_lista(l,*n,x,y-1,z);//marca os vertices adjacentes
		//inserir_lista(l,*n,y,x-1);	//usa-se se for gr�fico n�o direcionado
		i++;
	}while(i<arest);
	
	fclose(arq);
	printf("Arquivo fechado!\n");
	
	return l;
}

int visita(lista *l, int n, int x,int tempo){
	l[x].cor='C';	//troca para cinza
	elemento *aux=l[x].inicio;
	tempo++;
	l[x].d=tempo;	//adiciona tempo de descoberta
	while(aux!=NULL){
		if(l[aux->aresta].cor=='B'){	
			tempo=visita(l,n,aux->aresta,tempo);	//visita todos os vertices adjacentes que s�o brancos
		}
		aux=aux->prox;
	}
	l[x].cor='P';	//troca para preto
	tempo++;
	l[x].f=tempo;	//define tempo de finaliza��o
	return tempo;
}

void busca_p(lista *l, int n, int s){
	int i,tempo=0;
	
	for(i=0;i<n;i++){	//coloca todos em branco para come�ar busca
		l[i].cor='B';
	}
	for(i=-1;i<n;i++){	//percorre todos os vertices
		if(i==-1 && l[s].cor=='B'){
			tempo=visita(l,n,s,tempo);
		}
		else if(l[i].cor=='B'){
			tempo=visita(l,n,i,tempo);	//visita vertices brancos
		}
	}
}

fila* criar_fila(){
	fila *f=(fila*)malloc(sizeof(fila));
	f->inicio=NULL;
	f->final=NULL;
}

void inserir_fila(fila *f,int x){
	elemento *novo=(elemento*)malloc(sizeof(elemento));
	
	novo->aresta=x;
	if(f->inicio==NULL){
		f->inicio=novo;
		novo->prox=NULL;
		f->final=novo;
	}
	else{
		f->final->prox=novo;
		f->final=novo;
	}
}

void remover_fila(fila *f,int *x){
	if(f->final!=NULL){ //n�o esta vazia
		elemento *aux;
		*x=f->inicio->aresta;
		aux=f->inicio;
		f->inicio=f->inicio->prox;
		free(aux);
	}
}

void busca_l(lista *l, int n, int s){
	int i,u;
	elemento *aux;
	for(i=0;i<n;i++){	//define cor em Branco, distancia tendendo a infinito, e pai NULO
		l[i].cor='B';
		l[i].d=9999;
		l[i].p=-1;
	}
	l[s].cor='C'; //exceto para o inicio, que recebe cor Cinza e dist�ncia 0
	l[s].d=0;
	
	fila *q=criar_fila();
	inserir_fila(q,s); //cria fila e insere o inicio
	while(q->inicio!=NULL){	//enquanto q n�o for vazio
		remover_fila(q,&u); //remove primeiro item da fila
		aux=l[u].inicio;  //usa item como v�rtice
		while(aux!=NULL){
			if(l[aux->aresta].cor=='B'){ //se for branco
				l[aux->aresta].cor='C';
				l[aux->aresta].d=l[u].d+1;
				l[aux->aresta].p=u;
				inserir_fila(q,aux->aresta);
			}
			aux=aux->prox;
		}
		l[u].cor='P'; //define em preto quando todos os adjacentes j� foram descobertos
	}
	free(q);
}

conjunto* criar_conj(int max){
	conjunto *c=(conjunto*)malloc(sizeof(conjunto));
	c->fim=0;
	c->x=(int*)malloc(sizeof(int));
}

void inserir_conj(conjunto *c, int max, int x){
	if(c->fim<max){
		c->x[c->fim]=x;
		c->fim++;
	}
}

void remover_conj(conjunto *c,int *x, int pos){
	int aux,i;
	if(c->fim!=0){
		*x=c->x[pos];
		for(i=pos+1;i<c->fim;i++){
			c->x[i-1]=c->x[i];
		}
		c->fim--;
	}
}

int minimo(lista *l,int n,conjunto *c){
	int i,w,pos,min;
	for(i=0;i<c->fim;i++){ //percorre conjunto
		if(i==0){
			w=l[0].d;
			pos=0;
		}
		if(l[i].d<w){ //se dist�ncia do atual for menor, altera minimo
			w=l[i].d;
			pos=i;
		}
	}
	remover_conj(c,&min,pos); //remove o minimo pela posi��o encontrada
	return min;
}

void relax(lista *l, int n, int u, elemento *v){
	if(l[v->aresta].d>(l[u].d+v->peso)){ //verica se dist�ncia do v�rtice � maior que a dist�ncia do predecessor mais peso da aresta
		l[v->aresta].d=l[u].d+v->peso;
		l[v->aresta].p=u;
	}
}

void Dijkstra(lista *l, int n,int s){
	int i,u;
	elemento *aux;
	conjunto* q=criar_conj(n);
	
	for(i=0;i<n;i++){ //inicializa grafo
		l[i].d=9999;
		l[i].p=-1;
		inserir_conj(q,n,i);
	}
	l[s].d=0;
	while(q->fim>0){ //enquanto conjunto n�o for vazio
		u=minimo(l,n,q);
		aux=l[u].inicio;
		while(aux!=NULL){
			relax(l,n,u,aux);
			aux=aux->prox;
		}
	}
}

void encontrar_aresta(lista *l, int n, int *v, int *w, int a, int *p){
	int i,j=1,d=-1;
	elemento *aux;
	
	for(i=0;i<n;i++){ //percorre grafo
		aux=l[i].inicio;
		while(aux!=NULL){
			if(j==a){ //at� j for igual a ordem da aresta
				d=aux->aresta;
				*p=aux->peso;
				break;
			}
			aux=aux->prox;
			j++;
		}
		if(j==a && d!=-1){
			break;
		}
	}
	
	if(d!=-1){ //se aresta foi encontrada
		*v=i;
		*w=d;
	}
	else{ //se percorreu todo o grafo e aresta n�o foi encontrada
		*v=-1;
		*w=-1;
	}
}

int aresta_adj(lista *l,int n, int a){
	int i,j,k,p,check=-1;
	elemento *aux;
	
	encontrar_aresta(l,n,&i,&j,a,&p);
	
	if(i!=-1){ //se aresta foi encontrada
		printf("Aresta %d (V%d->V%d) � adjacente as arestas:\n",a,i+ST,j+ST);
		
		for(k=0;k<n;k++){
			aux=l[k].inicio;
			while(aux!=NULL){
				if(k==i && aux->aresta==j && aux->peso!=p){ //arestas adjacentes
					printf("V%d->V%d Peso=%d;\n",k+ST,aux->aresta+ST,aux->peso);
					check++;
				}
				
				aux=aux->prox;
			}
		}
		if(check==-1){
			printf("N�o existe aresta adjacente;\n");
		}
		return 1;
	}
	return 0;
}

void vertice_adj(lista *l,int n, int v){
	int i;
	elemento *aux;
	printf("Vertices adjacentes � V%d:\n",v+ST);
	
	aux=l[v].inicio;
	while(aux!=NULL){ //v�rtices que recebem aresta por v
		printf("V%d;\n",aux->aresta+ST);
		aux=aux->prox;
	}
	for(i=0;i<n;i++){
		if(i!=v){
			aux=l[i].inicio;
			while(aux!=NULL){
				if(aux->aresta==v){ //se aresta entra em v
					printf("V%d;\n",i+ST); //imprime origem
				}
				aux=aux->prox;
			}
		}
	}
}

void vertice_inc(lista *l,int n, int v){
	int i;
	elemento *aux;
	printf("Arestas incidentes em V%d:\n",v+ST);
	
	aux=l[v].inicio;
	while(aux!=NULL){ //arestas que saem de v
		printf("V%d->V%d Peso=%d;\n",v+ST,aux->aresta+ST,aux->peso);
		aux=aux->prox;
	}
	for(i=0;i<n;i++){
		if(i!=v){
			aux=l[i].inicio;
			while(aux!=NULL){
				if(aux->aresta==v){ //arestas que chegam em v
					printf("V%d->V%d Peso=%d;\n",i+ST,aux->aresta+ST,aux->peso);
				}
				aux=aux->prox;
			}
		}
	}
}

int aresta_inc(lista *l, int n, int a){
	int i,j,p;
	elemento *aux;
	
	encontrar_aresta(l,n,&i,&j,a,&p);
	
	if(i!=-1){
		printf("A aresta %d incide nos v�rtices V%d->V%d;\n",a,i+ST,j+ST); //v�rtices que a aresta sae e entra, respectivamente
		
		return 1;
	}
	return 0;
}

void grau(lista *l, int n, int v){
	int i,ge=0,gs=0;
	elemento *aux;
	
	aux=l[v].inicio;
	while(aux!=NULL){ //soma arestas de saida do v�rtice
		gs++;
		aux=aux->prox;
	}
	
	for(i=0;i<n;i++){
		if(i!=v){
			aux=l[i].inicio;
			while(aux!=NULL){
				if(aux->aresta==v){ //soma arestas de entrada em v
					ge++;
				}
				
				aux=aux->prox;
			}
		}
	}
	
	printf("V�rtice %d:\nGrau de Entrada = %d;\nGrau de Sa�da = %d;\n",v+ST,ge,gs);
}

int vertice_adj_2(lista *l, int n, int v, int w){
	int i;
	elemento *aux;
	
	aux=l[v].inicio;
	while(aux!=NULL){ //verifica arestas que saem de v
		if(aux->aresta==w){
			return 1; //v � a origem
		}
		
		aux=aux->prox;
	}
	
	aux=l[w].inicio;
	while(aux!=NULL){ //verifica arestas que saem de w
		if(aux->aresta==v){
			return -1; //w � a origem
		}
		
		aux=aux->prox;
	}
	return 0; //n�o s�o adjacentes
}

int trocar_peso(lista *l, int n, int a, int p){
	int i,j,w;
	elemento *aux;
	
	encontrar_aresta(l,n,&i,&j,a,&w);
	
	if(i!=-1){
		aux=l[i].inicio;
		while(aux!=NULL){
			if(aux->aresta==j){ //encontra aresta
				break;
			}
			
			aux=aux->prox;
		}
		aux->peso=p; //troca peso
		
		return 1;
	}
	return 0; //aresta n�o existe
}

int trocar_vertices(lista *l, int n, int v, int w){
	int i;
	lista hold;
	elemento *aux;
	if(v>=0 && v<n && w>=0 && w<n){
		aux=l[v].inicio;
		while(aux!=NULL){ //se v apontava para w, troca local
			if(aux->aresta==w){
				aux->aresta=v;
			}
			
			aux=aux->prox;
		}
		
		aux=l[w].inicio;
		while(aux!=NULL){ //se w apontava para v, troca local
			if(aux->aresta==v){
				aux->aresta=w;
			}
			
			aux=aux->prox;
		}
		
		for(i=0;i<n;i++){ //se algum outro v�rtice apontava para v ou w, troca local
			if(i!=v && i!=w){
				aux=l[i].inicio;
				while(aux!=NULL){
						if(aux->aresta==v){
							aux->aresta=w;
						}
						else if(aux->aresta==w){
							aux->aresta=v;
						}
					aux=aux->prox;
				}
			}
		}
		
		hold=l[v]; //troca inicio das listas
		l[v]=l[w];
		l[w]=hold;
		
		return 1;
	}
	return 0;
}

void menu(lista *l, int *n){
	int i,select=-1,x,y,z;
	while(select!=0){
		printf("**********************************\n\tTrabalho pr�tico 2\n**********************************\n\n");
		printf("Digite:\n\t1 para imprimir grafo;\n\t2 para imprimir arestas adjacentes a uma aresta;\n\t3 para imprimir v�rtices adjacentes a um v�rtice;\n\t4 para imprimir arestas incidentes a um v�rtice;\n\t5 para imprimir v�rtices incidentes a uma aresta;\n\t6 para imprimir o grau de um v�rtice;\n\t7 para determinar se 2 v�rtices s�o adjacentes;\n\t8 para substituir peso de uma aresta;\n\t9 para trocar 2 v�rtices;\n\t10 para Algor�tmo Dijkstra;\n\t11 para Busca em Largura;\n\t12 para Busca em Profundidade;\n\t0 para sair;\n\n");
		scanf("%d",&select);
		
		switch(select){
			case 0:
				printf("Saindo...\n");
				exit(0);
			break;
			
			case 1:
				imprimir(l,*n);
			break;
				
			case 2:
				printf("Qual aresta para adjac�ncia: ");
				scanf("%d",&x);
				
				z=aresta_adj(l,*n,x);
				
				if(z==0){
					printf("Aresta n�o existe no Grafo!\n");
				}
			break;
			
			case 3:
				printf("Qual v�rtice para adjac�ncia: ");
				scanf("%d",&x);
				x-=ST;
				
				if(x>=0 && x<*n){
					vertice_adj(l,*n,x);
				}
				else{
					printf("V�rtice n�o existe no Grafo!\n");
				}
			break;
			
			case 4:
				printf("Qual v�rtice para incid�ncia: ");
				scanf("%d",&x);
				x-=ST;
				
				if(x>=0 && x<*n){
					vertice_inc(l,*n,x);
				}
				else{
					printf("V�rtice n�o existe no Grafo!\n");
				}
			break;
			
			case 5:
				printf("Qual aresta para incid�ncia: ");
				scanf("%d",&x);
				
				z=aresta_inc(l,*n,x);
				
				if(z==0){
					printf("Aresta n�o existe no Grafo!\n");
				}
			break;
			
			case 6:
				printf("Calcular grau do v�rtice: ");
				scanf("%d",&x);
				x-=ST;
				
				if(x>=0 && x<*n){
					grau(l,*n,x);
				}
				else{
					printf("V�rtice n�o existe no Grafo!\n");
				}
			break;
			
			case 7:
				printf("V�rtices que ser�o verificados:\n");
				scanf("%d %d",&x,&y);
				x-=ST;
				y-=ST;
				
				if((x>=0 && x<*n) && (y>=0 && y<*n)){
					z=vertice_adj_2(l,*n,x,y);
					
					if(z==0){
						printf("V�rtices %d e %d n�o s�o adjacentes!\n",x+ST,y+ST);
					}
					else{
						printf("V�rtices %d e %d s�o adjacentes ",x+ST,y+ST);
						if(z==1){
							printf("pela aresta V%d->V%d!\n",x+ST,y+ST);
						}
						else{
							printf("pela aresta V%d->V%d!\n",y+ST,x+ST);
						}
					}
				}
				else{
					printf("Valores inv�lidos!\n");
				}
			break;
			
			case 8:
				printf("Digite a aresta e o peso a serem alterados, respectivamente:\n");
				scanf("%d %d",&x,&y);
				
				z=trocar_peso(l,*n,x,y);
				
				if(z==0){
					printf("Aresta n�o existe no Grafo!\n");
				}
			break;
			
			case 9:
				printf("Digite os v�rtices que ser�o trocados:\n");
				scanf("%d %d",&x,&y);
				x-=ST;
				y-=ST;
				if(x!=y){
				z=trocar_vertices(l,*n,x,y);
				
					if(z==0){
						printf("Valores inv�lidos!\n");
					}
					else{
						printf("Os v�rtices foram trocados!\n");
					}
				}
				else{
					printf("Os v�rtices escolhidos s�o iguais!\n");
				}
			break;
			
			case 10:
				printf("Escolha o v�rtice inicial: ");
				scanf("%d",&x);
				x-=ST;
				
				if(x>=0 && x<*n){
					Dijkstra(l,*n,x);
	
					printf("Algoritmo de Dijkstra com in�cio em V%d:\n",x+ST);
					for(i=0;i<*n;i++){
						if(l[i].p!=-1){
							printf("V�rtice %d:\nd = %d\np = %d\n\n",i+ST,l[i].d,l[i].p);
						}
						else{
							printf("V�rtice %d:\nd = %d\np = NULL\n\n",i+ST,l[i].d);
						}
					}
				}
				else{
					printf("V�rtice n�o existe no Grafo!\n");
				}
			break;
			
			case 11:
				printf("Escolha o v�rtice inicial: ");
				scanf("%d",&x);
				x-=ST; //as vezes funciona as vezes n�o, depende da execu��o
				
				if(x>=0 && x<*n){
					busca_l(l,*n,x);
		
					printf("BFS com in�cio em V%d:\n",x+ST);
					for(i=0;i<*n;i++){
						if(l[i].p!=-1){
							printf("V�rtice %d:\ncor = %c\nd = %d\np = %d\n\n",i+ST,l[i].cor,l[i].d,l[i].p);
						}
						else{
							printf("V�rtice %d:\ncor = %c\nd = %d\np = NULL\n\n",i+ST,l[i].cor,l[i].d);
							}
					}
				}
				else{
					printf("V�rtice n�o existe no Grafo!\n");
				}
			break;
			
			case 12:
				printf("Escolha o v�rtice inicial: ");
				scanf("%d",&x);
				x-=ST;
				
				if(x>=0 && x<*n){
					busca_p(l,*n,x);
		
					printf("DFS com in�cio em V%d:\n",x+ST);
					for(i=0;i<*n;i++){
						printf("V�rtice %d:\ncor = %c\nd = %d\nf = %d\n\n",i+ST,l[i].cor,l[i].d,l[i].f);
					}
				}
				else{
					printf("V�rtice n�o existe no Grafo!\n");
				}
			break;
			
			default:
				printf("Op��o Inv�lida!\n");
			break;
		}
		
		system("pause");
		system("cls");
	}
}
