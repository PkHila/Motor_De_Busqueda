#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diccionario.h"
#include "motorDeBusqueda.h"
#include "funcionesUsuarios.h"

void menu()
{
    printf("Inserte logo aqui\n");

    printf("\n1. Busqueda de un termino en algunos documentos");
    printf("\n2. Busqueda de un termino en todos los documentos");
    printf("\n3. Busqueda de varios terminos en un documento");
    printf("\n4. Busqueda de una frase");
    printf("\n5. Busqueda del termino mas frecuente en un documento");
    printf("\n6. Busqueda de un termino similar");
    printf("\n0. Salir");

    printf("\n\nIngrese su opcion: ");
}

int main()
{
    //!opcion 1
    nodoT* apariciones=NULL;
    char palabra[20]= {0};
    int verificacion=0;
    //!opcion 2
    int aparicionEnTodos=0;
    nodoId* listaId=NULL;
    //char palabra[20]= {0};
    //!opcion 3
    nodoPalabra* palabras=NULL;
    nodoPalabra* noEncontradas=NULL;
    int id=0;
    //!opcion 4

    //!opcion 5
    //int id=0;
    //!opcion 6

    //ayuda  a mostrar los docs
    int docAnt=0;

    int comando = -1;

    //DICCIONARIO
    cargarDiccionario();

    //ARBOL
    nodoA* arbol=NULL;
    cargarArbol(&arbol);

    //MENU
    while(comando!=0)
    {
        menu();
        fflush(stdin);
        scanf("%i",&comando);

        system("cls");

        switch(comando)
        {
        case 0:
            break;

        case 1:

            printf("\t ---------------------------------------------\n");
            printf("\t|             BUSQUEDA EN ALGUNOS             |\n");
            printf("\t ---------------------------------------------\n");

            verificacion=cargarPalabra(arbol,palabra);

            if(verificacion)
            {
                buscarAparicionesEnAlgunosDocs(arbol,palabra,&apariciones);
            }
            else
            {
                printf("\nLa palabra no se encuentra en ningun documento.\n");
            }

            break;

        case 2:

            printf("\t ---------------------------------------------\n");
            printf("\t|              BUSQUEDA EN TODOS              |\n");
            printf("\t ---------------------------------------------\n");

            verificacion=cargarIds(&listaId);

            if(verificacion)
            {
                verificacion=cargarPalabra(arbol,palabra);

                if(verificacion)
                {

                    aparicionEnTodos=buscarAparicionesEnTodosDocs(arbol,palabra,listaId,&apariciones);

                    if(!aparicionEnTodos)
                    {
                        printf("\nla palabra no se encuentra en todos los documentos indicados\n");
                    }
                }
                else
                {
                    printf("\nLa palabra no se encuentra en ningun documento.\n");
                }
            }
            else
            {
                printf("\nAlgunos de los id ingresados no son validos.\n");
            }

            break;

        case 3:
            printf("\t ---------------------------------------------\n");
            printf("\t|   BUSQUEDA VARIOS TERMINOS EN X DOCUMENTO   |\n");
            printf("\t ---------------------------------------------\n");

            cargarPalabras(arbol,&palabras,&noEncontradas);

            verificacion=ingresoIdDoc(&id);

            if(verificacion)
            {
                buscarVariasAparicionesEnXDoc(arbol,palabras,id,&apariciones);
                mostrarPalabrasNoEncontradas(noEncontradas);
            }
            else
            {
                printf("\nIdDoc no encontrado.\n");
            }

            break;

        case 4:
            printf("caso 4\n");

            break;

        case 5:
            printf("\t ---------------------------------------------\n");
            printf("\t|   BUSQUEDA TERMINO MAS FRECUENTE EN X DOC   |\n");
            printf("\t ---------------------------------------------\n");

            verificacion=ingresoIdDoc(&id);

            if(verificacion)
            {
                buscarPalabraMasFrecuente(arbol,id,&apariciones);
            }
            else
            {
                printf("\nIdDoc no encontrado.\n");
            }

            break;

        case 6:

            printf("\t ---------------------------------------------\n");
            printf("\t|             TERMINOS SIMILARES              |\n");
            printf("\t ---------------------------------------------\n");

            verificacion=cargarPalabra(arbol,palabra);

            if(verificacion){
                printf("\nLa palabra ingresada ya existe.\n");
            }else{
                sugerirSimilares(arbol,palabra);
            }

            break;
        }

        mostrarDocumento(apariciones,&docAnt);

        system("pause");
        system("cls");
    }

    return 0;
}
