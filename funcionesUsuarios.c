#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "motorDeBusqueda.h"

#include "funcionesUsuarios.h"

nodoId* crearNodoId(int dato)
{
    nodoId* nuevo=(nodoId*)malloc(sizeof(nodoId));
    nuevo->id=dato;
    nuevo->sig=NULL;
    return nuevo;
}

void mostrarListaId(nodoId* lista)
{
    while(lista){
        printf("\n%d ",lista->id);
        lista=lista->sig;
    }
}

/// 1) Buscar todas las apariciones de un término en algún documento (operación or).
/// (Tiene que estar en alguno de los textos)

void encontrarPalabra(nodoA* arbol,char palabra[20], nodoT** encontrado)
{
    if(arbol)
    {

        if(strcmpi(arbol->palabra,palabra)!=0)
        {

            if(strcmpi(arbol->palabra,palabra)<0)
            {
                encontrarPalabra(arbol->der,palabra,encontrado);
            }
            else
            {
                encontrarPalabra(arbol->izq,palabra,encontrado);
            }

        }
        else
        {
            *encontrado = arbol->ocurrencias;
        }
    }
    else
    {
        *encontrado = NULL;
    }
}

void buscarApariciones(nodoA* arbol,char palabra[20])
{
    nodoT* encontrado;
    encontrarPalabra(arbol, palabra, &encontrado);
    if(encontrado ==NULL)
    {
        printf("Sin resultados.\n");
    }
    else
    {
        printf("\n\tLISTA DE IDS");
        mostrarLista(encontrado);
    }
}

/// 2) Buscar todas las apariciones de un término en un documento y otros  (operacion and).
/// (Tiene que sí o sí estar en todos los textos que el usuario le indica)

int buscarIdXDocumento(nodoT* lista,nodoId* listaId)
{
    int flag=1;

    if(lista && listaId)
    {
        while(listaId && flag==1)//id ingresados por el usuario
        {
            while(lista && (lista->idDOC < listaId->id)) // lista del arbol
            {
                lista=lista->sig;
            }

            if(lista && (lista->idDOC == listaId->id)) //si se encuentra el id se pasa al siguiente
            {
                listaId=listaId->sig;
            }
            else //si no se encontro el id entonces se levanta la bandera
            {
                flag=0;
            }
        }
    }
    return flag;
}

void insertarId(nodoId** lista,int dato)//orden ascendente
{
    nodoId* nuevo=crearNodoId(dato);
    nodoId* ant, *seg;

    if(*lista)
    {

        if((*lista)->id > dato)
        {

            nuevo->sig=(*lista);
            (*lista)=nuevo;

        }
        else
        {

            ant=(*lista);
            seg=(*lista)->sig;

            while(seg && seg->id < dato)
            {
                ant=seg;
                seg=seg->sig;
            }

            ant->sig=nuevo;
            nuevo->sig=seg;
        }
    }else{

        (*lista)=nuevo;
    }
}

void buscarAparicionesXDocumento(nodoA* arbol,char palabra[20])
{
    int idIngresado=0;
    char control='s';
    nodoT* encontrado;
    int coincidencias=0;

    nodoId* lista=NULL;

    //primero encontrar la palabra-----------------------------
    encontrarPalabra(arbol, palabra, &encontrado);

    if(encontrado)
    {
        //segundo cargar una lista con los id que indica el usuario y ordenarla-------------------
        printf("Ingrese los id de los documentos donde desea realizar la busqueda: ");

        while(control=='s')
        {
            printf("Id: ");
            fflush(stdin);
            scanf("%d",&idIngresado);

            insertarId(&lista,idIngresado);

            printf("Seleccionar otro id? s/n: ");
            fflush(stdin);
            scanf("%c",&control);
        }

        //tercero buscar los id en la lista----------------------------------------

        coincidencias=buscarIdXDocumento(encontrado,lista);

        if(coincidencias)
        {
            printf("\n\tLISTA DE IDS");
            mostrarListaId(lista);
            printf("\nPalabras encontradas en todos los documentos");
        }
        else
        {
            printf("\nPalabra no encontrada en todos los documentos.");
        }
    }
    else
    {
        printf("Palabra no encontrada.");
    }
}
