#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {

  char palabra[20];
  int idDOC;
  int pos; //incrementa palabra por palabra, y no letra por letra

}termino;



void inicializarString(char**miString, long longitud)
{
    *miString = (char*)malloc(sizeof(longitud));
}

void cargarTextoAString(char* nombreTexto, char** stringTexto)
{
    long cant;
    FILE* arch = fopen(nombreTexto,"rb");
    if(arch)
    {
        fseek(arch, 0, SEEK_END);
        cant = ftell(arch);
        printf("el archivo tiene %ld bytes\n", cant);
        rewind(arch);
        inicializarString(stringTexto, cant+1);
        fread(*stringTexto, sizeof(char),cant,arch);
        printf("%s",*stringTexto);


    }
    fclose(arch);
}


int main()
{
    char* stringTotal;
    cargarTextoAString("Prueba.txt", &stringTotal);
    printf("%s", stringTotal);
    return 0;
}
