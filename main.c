#include <stdio.h>
#include <stdlib.h>

#define diccionario "diccionario.bin"

//!-----------------------------------ESTRUCTURA DEL DICCIONARIO----------------------------------------------------

typedef struct{

  char palabra[20];
  int idDOC;
  int pos; //incrementa palabra por palabra, y no letra por letra

}Termino;

//!-----------------------------------ESTRUCTURA DEL ARBOL----------------------------------------------------

typedef struct nodoA
{
   char palabra[20];
   int frecuencia; //representa la cantidad de nodos de la lista
   nodoT* ocurrencias; //ordenada por idDOC, luego por pos
   struct nodoA* der;
   struct nodoA* izq;
}nodoA;

typedef struct nodoT
{
    int idDOC;
    int pos;
    struct nodoT* sig;

}nodoT;

//!-----------------------------------FUNCIONES BASE----------------------------------------------------

nodoT* crearNodoT(Termino dato)
{
    nodoT* nuevo=(nodoT*)malloc(sizeof(nodoT));
    nuevo->idDOC=dato.idDOC;
    nuevo->pos=dato.pos;
    nuevo->sig=NULL;
    return nuevo;
}

//!-----------------------------------FUNCIONES----------------------------------------------------

void insertarNuevaOcurrencia(nodoT** lista,Termino dato)
{
    //primer criterio de orden ID, segundo POS

    nodoT* nuevo=crearNodoT(dato);

    if(*lista){

        if((*lista)->idDOC > dato.idDOC){

            nuevo->sig=(*lista);
            (*lista)=nuevo;
        }
    }
}

void insertarTerminoExistente(nodoA** arbol,Termino dato)
{
    //modificacion de la frecuencia
    *arbol->frecuencia+=1;

    //insercion en la lista
    insertarNuevaOcurrencia(&(*arbol)->ocurrencias,dato);
}

void buscarDato(nodoA* arbol,int dato,nodoA** encontrado)
{
    if(arbol){

        if(arbol->dato != dato){

            if(dato > arbol->dato){
                buscarDato(arbol->der,dato,encontrado);
            }else{
                buscarDato(arbol->izq,dato,encontrado);
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
    printf("Helloo!\n");
    return 0;
}
