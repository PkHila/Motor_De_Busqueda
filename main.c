#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "motorDeBusqueda.h"

int main()
{
    nodoA* arbol=NULL;
    cargarArbol(&arbol);

    mostrarArbol(arbol);

    return 0;
}
