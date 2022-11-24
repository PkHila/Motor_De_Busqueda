#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diccionario.h"

#define DICCIONARIO "diccionario.bin"
#define CANT_TEXTOS 10


termino crearTermino(char palabra[20], int idDocumento, int pos)
{
    termino nuevo;
    strcpy(nuevo.palabra,palabra);
    nuevo.idDOC = idDocumento;
    nuevo.pos = pos;
    return nuevo;
}

void cargarTextoAString(char* nombreTexto, char** textoCompleto) //copia la totalidad de un archivo de texto a una sola string.
{
    long cant;
    FILE* arch = fopen(nombreTexto,"rb");
    if(arch)
    {
        fseek(arch, 0, SEEK_END);
        cant = ftell(arch); //cant de bytes del texto
        *textoCompleto = (char*)calloc(cant, sizeof(char));
        rewind(arch);
        fread(*textoCompleto, sizeof(char),cant,arch);
    }
    fclose(arch);
}

int caracterValido(char c)
{
    //65 a 90 = Mayusculas, 97 a 122 = minusculas
    int valido= ((64 < c && c < 91) || (96 < c && c < 123)) ? 1 :  0;
    return valido;
}

void separarPalabras(char* textoCompleto, int idDocumento)
{
    char* palabra = (char*)calloc(20,sizeof(char));
    int i = 0; //indice de texto
    int j = 0; //indice de palabra individual
    int contadorPalabra = 1;
    FILE* arch = fopen(DICCIONARIO, "ab");
    if (arch)
    {
        while(i < strlen(textoCompleto)) //mientras el texto no haya terminado
        {
            if(caracterValido(textoCompleto[i])) //si el caracter es valido, se agrega a la nueva palabra
            {
                palabra[j] = textoCompleto[i];
                j++;
            }
            else
            {
                if(j>0)//si la palabra aumento de 0, quiere decir que es valida y se creara un nuevo termino
                {
                    termino nuevoTermino = crearTermino(palabra, idDocumento, contadorPalabra);
                    fwrite(&nuevoTermino, sizeof(termino),1,arch);
                    contadorPalabra++;
                    memset(palabra,0,strlen(palabra)); //vacia la palabra
                }
                j = 0;
            }
            i++;
        }
    }
    fclose(arch);
    free(palabra);
}

char* generarNombreArchivo(int idDocumento)
{
    char *nombreArch = (char*) calloc(20, sizeof(char));
    char idString[2];
    char cabecera[20] = "texto";
    char* extension = ".txt";
    sprintf(idString, "%i", idDocumento);
    strcat(cabecera, idString);
    strcat(cabecera, extension);
    strcpy(nombreArch, cabecera);
    return nombreArch;
}


void crearNuevoDiccionario()
{
    char* nombreArchivo, *stringTotal;
    int indice = 1; //inicia en 1 ya que no hay texto0
    while(indice <= CANT_TEXTOS)
    {
        nombreArchivo = generarNombreArchivo(indice);
        cargarTextoAString(nombreArchivo, &stringTotal);

        separarPalabras(stringTotal, indice);
        free(stringTotal);
        indice++;
    }

}

void cargarDiccionario()
{
    FILE* arch = fopen(DICCIONARIO,"rb");
    if(!arch)
    {
        fclose(arch);
        crearNuevoDiccionario();
    }
    else
    {
        fclose(arch);
    }
}

void mostrarTermino(termino registro)
{
    printf("%s", registro.palabra);
    printf("\nDoc num %i \nPos %i \n\n", registro.idDOC, registro.pos);
}

void mostrarDiccionario()
{
    termino aux;
    FILE* arch = fopen(DICCIONARIO,"rb");
    if(arch)
    {
        while(fread(&aux,sizeof(termino),1,arch)>0)
        {
            mostrarTermino(aux);
        }
    }
    fclose(arch);
}
