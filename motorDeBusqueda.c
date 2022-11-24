#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "motorDeBusqueda.h"

//!-----------------------------------FUNCIONES BASE----------------------------------------------------

nodoT* crearNodoT(int idDoc,int pos)
{
    nodoT* nuevo=(nodoT*)malloc(sizeof(nodoT));
    nuevo->idDOC=idDoc;
    nuevo->pos=pos;
    nuevo->sig=NULL;
    return nuevo;
}

nodoA* crearNodoA(Termino dato)
{
    nodoA* nuevo=(nodoA*)malloc(sizeof(nodoA));
    strcpy(nuevo->palabra,dato.palabra);
    nuevo->frecuencia=1;
    nuevo->ocurrencias=crearNodoT(dato.idDOC,dato.pos);
    nuevo->izq=NULL;
    nuevo->der=NULL;
    return nuevo;
}

void mostrarLista(nodoT* lista)
{
    while(lista)
    {
        printf("\nId: %d",lista->idDOC);
        printf("\nPos: %d",lista->pos);

        printf("\n.....................\n");

        lista=lista->sig;
    }
}

void mostrarArbol(nodoA* arbol)
{
    if(arbol)
    {
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
        if(strcmpi(dato.palabra,(*arbol)->palabra)>0)
        {
            insertarNuevoTermino(&(*arbol)->der,dato);
        }
        else
        {
            insertarNuevoTermino(&(*arbol)->izq,dato);
        }
    }
    else
    {
        (*arbol)=crearNodoA(dato);
    }
}

void insertarNuevaOcurrencia(nodoT** lista,int idDoc,int pos)//primer criterio de orden ID, segundo POS
{
    nodoT* nuevo=crearNodoT(idDoc,pos);
    nodoT* ant,* seg;

    if(*lista)
    {

        if((*lista)->idDOC > idDoc || ( (*lista)->idDOC == idDoc && (*lista)->pos > pos))
        {
            nuevo->sig=(*lista);
            (*lista)=nuevo;
        }
        else
        {
            ant=(*lista);
            seg=(*lista)->sig;


            while(seg && ( (seg->idDOC < idDoc) || (seg->idDOC == idDoc && seg->pos < pos) ) )
            {
                ant=seg;
                seg=seg->sig;
            }

            ant->sig=nuevo;
            nuevo->sig=seg;
        }
    }
    else
    {
        (*lista)=nuevo;
    }
}

void insertarTerminoExistente(nodoA** arbol,Termino dato)
{
    //modificacion de la frecuencia
    (*arbol)->frecuencia+=1;

    //insercion en la lista
    insertarNuevaOcurrencia(&(*arbol)->ocurrencias,dato.idDOC,dato.pos);
}

void existeTermino(nodoA* arbol,char palabra[],nodoA** encontrado)
{
    if(arbol)
    {

        if(strcmpi(arbol->palabra,palabra)!=0)
        {

            if(strcmpi(palabra,arbol->palabra)>0)
            {
                existeTermino(arbol->der,palabra,encontrado);
            }
            else
            {
                existeTermino(arbol->izq,palabra,encontrado);
            }
        }
        else
        {
            *encontrado=arbol;
        }
    }
}

void cargarTermino(nodoA** arbol,Termino aux)
{
    //fijarse si ya existe la palabra en el arbol
    nodoA* existencia=NULL;
    existeTermino(*arbol,aux.palabra,&existencia);

    if(existencia)  //si existe se le incrementa frecuencia y ademas agregar un nodo a la lista
    {

        insertarTerminoExistente(&existencia,aux);

    }
    else   //sino se crea un nuevo nodo en el arbol
    {

        insertarNuevoTermino(arbol,aux);
    }
}

void cargarArbol(nodoA** arbol)
{
    Termino aux;
    FILE* arch = fopen(diccionario,"rb");

    if(arch)
    {

        while(fread(&aux,sizeof(Termino),1,arch)>0)
        {

            cargarTermino(arbol,aux);

        }

        fclose(arch);
    }
}
