#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "diccionario.h"
#include "motorDeBusqueda.h"
#include "funcionesUsuarios.h"


int main()
{
    //DICCIONARIO
    cargarDiccionario();

    //ARBOL
    nodoA* arbol=NULL;
    cargarArbol(&arbol);

    //mostrarArbol(arbol);


    //USUARIO
    nodoT* apariciones=NULL;
    buscarFrase(arbol, &apariciones);


    return 0;
}
