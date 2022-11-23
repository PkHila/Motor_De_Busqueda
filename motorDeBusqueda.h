#ifndef MOTORDEBUSQUEDA_H_INCLUDED
#define MOTORDEBUSQUEDA_H_INCLUDED

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


//!-----------------------------------FUNCIONES----------------------------------------------------

nodoT* crearNodoT(int idDoc,int pos);
nodoA* crearNodoA(Termino dato);
void mostrarLista(nodoT* lista);
void mostrarArbol(nodoA* arbol);
void insertarNuevoTermino(nodoA** arbol,Termino dato);
void insertarNuevaOcurrencia(nodoT** lista,int idDoc,int pos);
void insertarTerminoExistente(nodoA** arbol,Termino dato);
void existeTermino(nodoA* arbol,char palabra[],nodoA** encontrado);
void cargarTermino(nodoA** arbol,Termino aux);
void cargarArbol(nodoA** arbol);


#endif // MOTORDEBUSQUEDA_H_INCLUDED
