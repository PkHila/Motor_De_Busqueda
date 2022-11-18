#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{

    char palabra[20];
    int idDOC;
    int pos; //incrementa palabra por palabra, y no letra por letra

} termino;




void inicializarString(char*miString[], long longitud) //inicializa la string que contiene la totalidad del texto segun el tamaño del archivo
{
    *miString = (char*)malloc(longitud * sizeof(char));
}



void cargarTextoAString(char* nombreTexto, char* stringTexto[]) //copia la totalidad de un archivo de texto a una sola string.
{
    long cant;
    FILE* arch = fopen(nombreTexto,"rb");
    if(arch)
    {
        fseek(arch, 0, SEEK_END);
        cant = ftell(arch); //cant de bytes del texto
        inicializarString(stringTexto, cant);
        rewind(arch);
        fread(*stringTexto, sizeof(char),cant,arch);
    }
    fclose(arch);
}

termino crearTermino(char palabra[20], int idDocumento, int pos)
{
    termino nuevo;
    strcpy(nuevo.palabra,palabra);
    nuevo.idDOC = idDocumento;
    nuevo.pos = pos;
    return nuevo;
}

int caracterValido(char c)
{

    if(31 < c && c < 48)
    {
        return 0;
    }
    else
    {
       return 1;
    }

}

void separarPalabras(char* stringTotal, int idDocumento)
{
    char palabra[20];
    int i = 0;
    int j = 0;

    while(i < strlen(stringTotal)) //mientras el texto no haya terminado
    {
        if(caracterValido(stringTotal[i]))
        {
            palabra[j] = stringTotal[i];
            j++;
        }
        else
        {
            j = 0;
            termino nuevoTermino = crearTermino(palabra, idDocumento, i);
        }
        i++;
    }
}

void generarNombreArchivo(char* nombreArch[20], int id)
{
    char idString[2];
    char cabecera[20] = "texto";
    char* extension = ".bin";
    sprintf(idString, "%i", id);
    strcat(cabecera, idString);
    strcat(cabecera, extension);
    *nombreArch = cabecera;
}


void crearDiccionario(int idMaxima)
{
    FILE* arch;
    char* nombreArchivo, *stringTotal;
    int indice = 1; //inicia en 1 ya que no hay texto0
    while(indice <= idMaxima)
    {
        generarNombreArchivo(&nombreArchivo, indice);
        arch = fopen(nombreArchivo,"rb");
        if(arch)
        {
            cargarTextoAString(nombreArchivo, &stringTotal);
            separarPalabras(stringTotal, indice);

        }
        fclose(arch);

        indice++;
    }

}


int main()
{
    char* stringTotal = NULL;
    cargarTextoAString("Prueba.txt", &stringTotal);
    printf("%s\n", stringTotal);
    system("pause");

    ///Prueba de nombre de archivo
    printf("---------------Prueba de nombre de archivo -------------\n");
    int a = 1;
    char* nombre;
    generarNombreArchivo(&nombre, a);
    printf("%s\n", nombre);

    ///Prueba de caracteres validos
    char c1 = '!';
    char c2 = 'X';
    char c3 = 'b';
    char c4 = ' ';
    printf("c1 %c = %i\n", c1, caracterValido(c1));
    printf("c2 %c = %i\n", c2, caracterValido(c2));
    printf("c3 %c = %i\n", c3, caracterValido(c3));
    printf("c4 %c = %i\n", c4, caracterValido(c4));

    return 0;
}
