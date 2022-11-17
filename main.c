#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {

  char palabra[20];
  int idDOC;
  int pos; //incrementa palabra por palabra, y no letra por letra

}termino;



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
        rewind(arch);
        inicializarString(stringTexto, cant);
        fread(*stringTexto, sizeof(char),cant,arch);
    }
    fclose(arch);
}


int main()
{
    char* stringTotal = NULL;
    cargarTextoAString("Prueba.txt", &stringTotal);
    printf("%s", stringTotal);
    return 0;
}
