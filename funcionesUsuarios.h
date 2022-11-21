#ifndef FUNCIONESUSUARIOS_H_INCLUDED
#define FUNCIONESUSUARIOS_H_INCLUDED

typedef struct nodoId
{
    int id;
    struct nodoId* sig;

} nodoId;

nodoId* crearNodoId(int dato);
void mostrarListaId(nodoId* lista);
void encontrarPalabra(nodoA* arbol,char palabra[20], nodoT** encontrado);
void buscarApariciones(nodoA* arbol,char palabra[20]);
int buscarIdXDocumento(nodoT* lista,nodoId* listaId);
void insertarId(nodoId** lista,int dato);
void buscarAparicionesXDocumento(nodoA* arbol,char palabra[20]);

#endif // FUNCIONESUSUARIOS_H_INCLUDED
