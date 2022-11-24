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

    mostrarArbol(arbol);

    //USUARIO
    nodoT* apariciones=NULL;

    /*
    printf("\t ---------------------------------------------\n");
    printf("\t|             BUSQUEDA EN ALGUNOS             |\n");
    printf("\t ---------------------------------------------\n");

    buscarAparicionesEnAlgunosDocs(arbol,"salame",&apariciones);
    mostrarLista(apariciones);
    */

    /*
    printf("\t ---------------------------------------------\n");
    printf("\t|              BUSQUEDA EN TODOS              |\n");
    printf("\t ---------------------------------------------\n");

    int aparicionEnTodos=0;

    nodoId* listaId=NULL;
    cargarIds(&listaId);

    aparicionEnTodos=buscarAparicionesEnTodosDocs(arbol,"salame",listaId,&apariciones);

    if(aparicionEnTodos)
    {
        mostrarLista(apariciones);
    }else{

        printf("\nla palabra no se encuentra en todos los documentos indicados\n");
    }
    */

    //printf("\t ---------------------------------------------\n");
    //printf("\t|   BUSQUEDA VARIOS TERMINOS EN X DOCUMENTO   |\n");
    //printf("\t ---------------------------------------------\n");

    /*
    nodoPalabra* palabras=NULL;
    cargarPalabras(arbol,&palabras);

    int id=0;
    printf("Ingrese el id donde desea realizar la busqueda: ");
    fflush(stdin);
    scanf("%d",&id);

    //!VALIDACION DE ID

    buscarVariasAparicionesEnXDoc(arbol,palabras,id,&apariciones);
    mostrarLista(apariciones);
    */


    printf("\t ---------------------------------------------\n");
    printf("\t|   BUSQUEDA TERMINO MAS FRECUENTE EN X DOC   |\n");
    printf("\t ---------------------------------------------\n");

    int id=0;
    printf("Ingrese el id donde desea realizar la busqueda: ");
    fflush(stdin);
    scanf("%d",&id);

    //!VALIDACION DE ID

    buscarPalabraMasFrecuente(arbol,id,&apariciones);

    //mostrarLista(apariciones);

    mostrarDocumento(apariciones);


    return 0;
}
