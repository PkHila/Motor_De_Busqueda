#ifndef FUNCIONESUSUARIOS_H_INCLUDED
#define FUNCIONESUSUARIOS_H_INCLUDED

#include "motorDeBusqueda.h"

typedef struct nodoId
{
    int id;
    struct nodoId* sig;

} nodoId;

typedef struct nodoPalabra
{
    char palabra[20];
    struct nodoPalabra* sig;

} nodoPalabra;

nodoId* crearNodoId(int dato);
void mostrarListaId(nodoId* lista);
nodoPalabra* crearNodoPalabra(char dato[20]);
void mostrarListaPlabras(nodoPalabra* lista);
void insertarId(nodoId** lista,int dato);//orden ascendente
void cargarIds(nodoId** lista);
void insertarPalabra(nodoPalabra** lista,char palabra[]);
int verificarPalabra(nodoA* arbol,char palabra[]);
void cargarPalabras(nodoA* arbol,nodoPalabra** lista);
void buscarAparicionesEnAlgunosDocs(nodoA* arbol,char palabra[20],nodoT** apariciones);
int coincideId(nodoT* lista,int idBuscado,nodoT** apariciones);
int buscarAparicionesEnTodosDocs(nodoA* arbol,char palabra[],nodoId* idBuscado,nodoT** apariciones);
void buscarVariasAparicionesEnXDoc(nodoA* arbol,nodoPalabra* palabras,int id,nodoT** ocurrencias);
int sumarApariciones(nodoT* lista,int id);
void encontrarPalabraMasFrecuente(nodoA* arbol,int id,char palabra[],int* maxApariciones);
void buscarPalabraMasFrecuente(nodoA* arbol,int id,nodoT** apariciones);
void mostrarPalabra(Termino temp,nodoT* lista);
void mostrarDocumento(nodoT* lista);
void ingresarFrase(char* frase);
void separarFrase(nodoA * arbol, char* frase, char arregloPalabras[][20], int* validos);
void buscarPalabrasContinuas(nodoA** arregloNodosA,int validos);
void buscarFrase(nodoA* arbol);

#endif // FUNCIONESUSUARIOS_H_INCLUDED
