#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define diccionario "diccionario.bin"

//!-----------------------------------ESTRUCTURA DEL DICCIONARIO----------------------------------------------------

typedef struct{

  char palabra[20];
  int idDOC;
  int pos; //incrementa palabra por palabra, y no letra por letra

}Termino;

//!-----------------------------------ESTRUCTURA DEL ARBOL----------------------------------------------------

typedef struct nodoT
{
    int idDOC;
    int pos;
    struct nodoT* sig;

}nodoT;

typedef struct nodoA
{
   char palabra[20];
   int frecuencia; //representa la cantidad de nodos de la lista
   nodoT* ocurrencias; //ordenada por idDOC, luego por pos
   struct nodoA* der;
   struct nodoA* izq;
}nodoA;

//!-----------------------------------FUNCIONES BASE----------------------------------------------------

nodoT* crearNodoT(Termino dato)
{
    nodoT* nuevo=(nodoT*)malloc(sizeof(nodoT));
    nuevo->idDOC=dato.idDOC;
    nuevo->pos=dato.pos;
    nuevo->sig=NULL;
    return nuevo;
}

nodoA* crearNodoA(Termino dato)
{
    nodoA* nuevo=(nodoA*)malloc(sizeof(nodoA));
    strcpy(nuevo->palabra,dato.palabra);
    nuevo->frecuencia=1;
    nuevo->ocurrencias=crearNodoT(dato);
    nuevo->izq=NULL;
    nuevo->der=NULL;
    return nuevo;
}

void mostrarLista(nodoT* lista)
{
    while(lista){

        printf("\nId: %d",lista->idDOC);
        printf("\nPos: %d",lista->pos);

        printf("\n.....................\n");

        lista=lista->sig;
    }
}

void mostrarArbol(nodoA* arbol)
{
    if(arbol){
        mostrarArbol(arbol->izq);
        printf("\nPalabra: %s",arbol->palabra);
        printf("\nFrecuencia: %d",arbol->frecuencia);
        mostrarLista(arbol->ocurrencias);
        printf("\n\n-------------------\n\n");
        mostrarArbol(arbol->der);
    }
}

//!-----------------------------------FUNCIONES----------------------------------------------------

void insertarNuevoTermino(nodoA** arbol,Termino dato)
{
     if(*arbol)
    {
        if(strcmp(dato.palabra,(*arbol)->palabra)>0)
        {
            insertarNuevoTermino(&(*arbol)->der,dato);
        }
        else
        {
            insertarNuevoTermino(&(*arbol)->izq,dato);
        }
    }else{

        (*arbol)=crearNodoA(dato);
    }

}

void insertarNuevaOcurrencia(nodoT** lista,Termino dato)//primer criterio de orden ID, segundo POS
{
    nodoT* nuevo=crearNodoT(dato);
    nodoT* ant,* seg;

    if(*lista){

        if((*lista)->idDOC > dato.idDOC || ( (*lista)->idDOC == dato.idDOC && (*lista)->pos > dato.pos)){

            nuevo->sig=(*lista);
            (*lista)=nuevo;

        }else{

            ant=(*lista);
            seg=(*lista)->sig;

            while(seg && ( (seg->idDOC < dato.idDOC) || (seg->idDOC == dato.idDOC && seg->pos < dato.pos) ) ){

                ant=seg;
                seg=seg->sig;
            }

            ant->sig=nuevo;
            nuevo->sig=seg;
        }
    }
}

void insertarTerminoExistente(nodoA** arbol,Termino dato)
{
    //modificacion de la frecuencia
    (*arbol)->frecuencia+=1;

    //insercion en la lista
    insertarNuevaOcurrencia(&(*arbol)->ocurrencias,dato);
}

void existeTermino(nodoA* arbol,char palabra[],nodoA** encontrado)
{
    if(arbol){

        if(strcmp(arbol->palabra,palabra)!=0){

            if(strcmp(palabra,arbol->palabra)>0){
                existeTermino(arbol->der,palabra,encontrado);
            }else{
                existeTermino(arbol->izq,palabra,encontrado);
            }
        }else{
            *encontrado=arbol;
        }
    }
}

void cargarTermino(nodoA** arbol,Termino aux)
{
    //fijarse si ya existe la palabra en el arbol
    nodoA* existencia=NULL;
    existeTermino(*arbol,aux.palabra,&existencia);

    if(existencia){ //si existe se le incrementa frecuencia y ademas agregar un nodo a la lista

        insertarTerminoExistente(&existencia,aux);

    }else{ //sino se crea un nuevo nodo en el arbol

        insertarNuevoTermino(arbol,aux);
    }
}

void cargarArbol(nodoA** arbol)
{
    Termino aux;
    FILE* arch = fopen(diccionario,"rb");

    if(arch){

        while(fread(&aux,sizeof(Termino),1,arch)>0){

            cargarTermino(arbol,aux);

        }

        fclose(arch);
    }
}

int main()
{
    nodoA* arbol=NULL;
    cargarArbol(&arbol);

    //mostrarArbol(arbol);

    return 0;
}
