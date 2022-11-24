#ifndef FUNCIONESUSUARIOS_H_INCLUDED
#define FUNCIONESUSUARIOS_H_INCLUDED

#include "motorDeBusqueda.h"
#include "diccionario.h"

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

int cargarPalabra(nodoA* arbol,char palabra[]);
nodoId* crearNodoId(int dato);
void mostrarListaId(nodoId* lista);
nodoPalabra* crearNodoPalabra(char dato[20]);
void mostrarListaPlabras(nodoPalabra* lista);
void insertarId(nodoId** lista,int dato);//orden ascendente
int cargarIds(nodoId** lista);
void insertarPalabra(nodoPalabra** lista,char palabra[]);
int verificarPalabra(nodoA* arbol,char palabra[]);
void cargarPalabras(nodoA* arbol,nodoPalabra** lista,nodoPalabra** noEncontradas);
int ingresoIdDoc(int* idDoc);
void buscarAparicionesEnAlgunosDocs(nodoA* arbol,char palabra[20],nodoT** apariciones);
int coincideId(nodoT* lista,int idBuscado,nodoT** apariciones);
int buscarAparicionesEnTodosDocs(nodoA* arbol,char palabra[],nodoId* idBuscado,nodoT** apariciones);
void buscarVariasAparicionesEnXDoc(nodoA* arbol,nodoPalabra* palabras,int id,nodoT** ocurrencias);
int sumarApariciones(nodoT* lista,int id);
void encontrarPalabraMasFrecuente(nodoA* arbol,int id,char palabra[],int* maxApariciones);
void buscarPalabraMasFrecuente(nodoA* arbol,int id,nodoT** apariciones);
void ingresarFrase(char* frase);
void separarFrase(nodoA * arbol, char* frase, char arregloPalabras[][20], int* validos);
void buscarPalabrasContinuas(nodoT** arreglo,int validos, nodoT** apariciones);
void buscarFrase(nodoA* arbol, nodoT** apariciones);
int Minimo(int a, int b);
int Levenshtein(char *s1,char *s2);
void sugerirSimilares(nodoA* arbol,char palabra[]);
void mostrarPalabrasNoEncontradas(nodoPalabra* noEncontradas);
int mostrarPalabra(Termino temp,nodoT* lista,int avisoImpresion);
void mostrarDocumento(nodoT* lista,int* docAnt);

#endif // FUNCIONESUSUARIOS_H_INCLUDED
