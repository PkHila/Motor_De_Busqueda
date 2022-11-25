#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "diccionario.h"
#include "motorDeBusqueda.h"
#include "funcionesUsuarios.h"


//!-----------------------------------COLORES DE CONSOLA----------------------------------------------

#define RESET_COLOR   "\x1b[0m"
#define NEGRO_T       "\x1b[30m"
#define NEGRO_F       "\x1b[40m"
#define ROJO_T        "\x1b[31m"
#define ROJO_F        "\x1b[41m"
#define VERDE_T       "\x1b[32m"
#define VERDE_F       "\x1b[42m"
#define AMARILLO_T    "\x1b[33m"
#define AMARILLO_F    "\x1b[43m"
#define AZUL_T        "\x1b[34m"
#define AZUL_F        "\x1b[44m"
#define MAGENTA_T     "\x1b[35m"
#define MAGENTA_F     "\x1b[45m"
#define CYAN_T        "\x1b[36m"
#define CYAN_F        "\x1b[46m"
#define BLANCO_T      "\x1b[37m"
#define BLANCO_F      "\x1b[47m"


//!-----------------------------------PROTOTIPOS MENU----------------------------------------------

void dibujarLogo();
int menuAnimado();
int flecha (int teclaApretada, int posicion);

//!-----------------------------------     M A I N      ---------------------------------------------

int main()
{
    int comando = -1;

    //DICCIONARIO
    cargarDiccionario();

    //ARBOL
    nodoA* arbol=NULL;
    cargarArbol(&arbol);

    //MENU
    while(comando!=7)
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

        comando=menuAnimado();
        system("cls");

        switch(comando)
        {
        case 1:

            printf(CYAN_T"\t ---------------------------------------------\n");
            printf("\t|             BUSQUEDA EN ALGUNOS             |\n");
            printf("\t ---------------------------------------------\n"RESET_COLOR);

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

            printf(CYAN_T"\t ---------------------------------------------\n");
            printf("\t|              BUSQUEDA EN TODOS              |\n");
            printf("\t ---------------------------------------------\n"RESET_COLOR);

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
            printf(CYAN_T"\t ---------------------------------------------\n");
            printf("\t|   BUSQUEDA VARIOS TERMINOS EN X DOCUMENTO   |\n");
            printf("\t ---------------------------------------------\n"RESET_COLOR);

            cargarPalabras(arbol,&palabras,&noEncontradas);

            verificacion=ingresoIdDoc(&id);

            if(verificacion)
            {
                buscarVariasAparicionesEnXDoc(arbol,palabras,id,&apariciones,&noEncontradas);
                mostrarPalabrasNoEncontradas(noEncontradas);
            }
            else
            {
                printf("\nIdDoc no encontrado.\n");
            }

            break;

        case 4:
            printf(CYAN_T"\t ---------------------------------------------\n");
            printf("\t|         BUSQUEDA DE FRASE COMPLETA          |\n");
            printf("\t ---------------------------------------------\n"RESET_COLOR);

            buscarFrase(arbol, &apariciones);

            break;

        case 5:
            printf(CYAN_T"\t ---------------------------------------------\n");
            printf("\t|   BUSQUEDA TERMINO MAS FRECUENTE EN X DOC   |\n");
            printf("\t ---------------------------------------------\n"RESET_COLOR);

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

            printf(CYAN_T"\t ---------------------------------------------\n");
            printf("\t|             TERMINOS SIMILARES              |\n");
            printf("\t ---------------------------------------------\n"RESET_COLOR);

            verificacion=cargarPalabra(arbol,palabra);

            if(verificacion)
            {
                printf("\nLa palabra ingresada ya existe.\n");
            }
            else
            {
                sugerirSimilares(arbol,palabra);
            }

            break;

        default:

            break;
        }

        mostrarDocumento(apariciones,&docAnt);

        if(comando!=7)
        {
            system("pause");
        }
        system("cls");
    }

    return 0;
}

//!-----------------------------------FUNCIONES MENU----------------------------------------------

void dibujarLogo()
{
    printf("                                                 ");
    printf(VERDE_T",,  \n"RESET_COLOR);
    printf(CYAN_T"  .g8'''bgd                                    "RESET_COLOR);
    printf(VERDE_T"`7MM  \n"RESET_COLOR);
    printf(CYAN_T".dP'     `M                                      "RESET_COLOR);
    printf(VERDE_T"MM  \n"RESET_COLOR);

    printf(CYAN_T"dM'       `  "RESET_COLOR);
    printf(ROJO_T"`7MM  `7MM   "RESET_COLOR);
    printf(AMARILLO_T".P'Ybmmm "RESET_COLOR);
    printf(CYAN_T"`7MM  `7MM    "RESET_COLOR);
    printf(VERDE_T"MM \n"RESET_COLOR);

    printf(CYAN_T"MM             "RESET_COLOR);
    printf(ROJO_T"MM    MM  "RESET_COLOR);
    printf(AMARILLO_T":MI  I8     "RESET_COLOR);
    printf(CYAN_T"MM    MM    "RESET_COLOR);
    printf(VERDE_T"MM \n"RESET_COLOR);

    printf(CYAN_T"MM.    `7MMF'  "RESET_COLOR);
    printf(ROJO_T"MM    MM   "RESET_COLOR);
    printf(AMARILLO_T"WmmmP'     "RESET_COLOR);
    printf(CYAN_T"MM    MM    "RESET_COLOR);
    printf(VERDE_T"MM  \n"RESET_COLOR);

    printf(CYAN_T"`Mb.     MM    "RESET_COLOR);
    printf(ROJO_T"MM    MM  "RESET_COLOR);
    printf(AMARILLO_T"8M          "RESET_COLOR);
    printf(CYAN_T"MM    MM    "RESET_COLOR);
    printf(VERDE_T"MM \n"RESET_COLOR);


    printf(CYAN_T"  `'bmmmdPY    "RESET_COLOR);
    printf(ROJO_T"`Mbod'YML. "RESET_COLOR);
    printf(AMARILLO_T"YMMMMMb    "RESET_COLOR);
    printf(CYAN_T"`Mbod'YML."RESET_COLOR);
    printf(VERDE_T".JMML.\n"RESET_COLOR);

    printf(AMARILLO_T"                         6'     dP                   \n");
    printf("                         Ybmmmd'                     \n\n"RESET_COLOR);
}

int menuAnimado()
{
    system ("cls");
    int posicion = 1;
    int teclaApretada = 0;

    while(teclaApretada != 13)
    {
        system ("cls");

        dibujarLogo();

        if(posicion == 1)
        {
            printf(NEGRO_T BLANCO_F"Busqueda de un termino en algunos documentos\n"RESET_COLOR);
        }
        else
        {
            printf("Busqueda de un termino en algunos documentos\n");
        }

        if(posicion == 2)
        {
            printf(NEGRO_T BLANCO_F"Busqueda de un termino en todos los documentos\n"RESET_COLOR);
        }
        else
        {
            printf("Busqueda de un termino en todos los documentos\n");
        }

        if(posicion == 3)
        {
            printf(NEGRO_T BLANCO_F"Busqueda de varios terminos en un documento\n"RESET_COLOR);
        }
        else
        {
            printf("Busqueda de varios terminos en un documento\n");
        }

        if(posicion == 4)
        {
            printf(NEGRO_T BLANCO_F"Busqueda de una frase\n"RESET_COLOR);
        }
        else
        {
            printf("Busqueda de una frase\n");
        }

        if(posicion == 5){

            printf(NEGRO_T BLANCO_F"Busqueda del termino mas frecuente en un documento \n"RESET_COLOR);

        }else{

            printf("Busqueda del termino mas frecuente en un documento\n");
        }

        if(posicion == 6){

            printf(NEGRO_T BLANCO_F"Busqueda de un termino similar\n"RESET_COLOR);

        }else{

            printf("Busqueda de un termino similar\n");
        }

        if(posicion == 7){

            printf(NEGRO_T BLANCO_F"Salir\n"RESET_COLOR);

        }else{

            printf("Salir\n");
        }

        teclaApretada = getch();

        posicion = flecha(teclaApretada, posicion);
    }

    return posicion;
}

int flecha (int teclaApretada, int posicion)
{
    if(teclaApretada == 80 && posicion != 7)
    {
        posicion++;
    }
    else if(teclaApretada == 72 && posicion != 1)
    {
        posicion--;
    }
    else if(teclaApretada == 27)
    {
        menuAnimado();
    }

    return posicion;
}
