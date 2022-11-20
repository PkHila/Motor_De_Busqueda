#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICCIONARIO "diccionario.bin"

typedef struct
{

    char palabra[20];
    int idDOC;
    int pos; //incrementa palabra por palabra, y no letra por letra

} termino;


void cargarTextoAString(char* nombreTexto, char* stringTexto[]) //copia la totalidad de un archivo de texto a una sola string.
{
    long cant;
    FILE* arch = fopen(nombreTexto,"rb");
    if(arch)
    {
        fseek(arch, 0, SEEK_END);
        cant = ftell(arch); //cant de bytes del texto
        *stringTexto = (char*)calloc(cant, sizeof(char));
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
    int valido= ((64 < c && c < 91) || (96 < c && c < 123)) ? 1 :  0;
    return valido;
}

void separarPalabras(char* stringTotal, int idDocumento)
{
    char* palabra = (char*)calloc(20,sizeof(char));
    int i = 0; //indice de texto
    int j = 0; //indice de palabra individual
    FILE* arch = fopen(DICCIONARIO, "ab");
    if (arch)
    {
        while(i < strlen(stringTotal)) //mientras el texto no haya terminado
        {
            if(caracterValido(stringTotal[i])) //si el caracter es valido, se agrega a la nueva palabra
            {
                palabra[j] = stringTotal[i];
                j++;
            }
            else
            {
                if(j>0)//si la palabra aumento de 0, quiere decir que es valida y se creara un nuevo termino
                {
                    termino nuevoTermino = crearTermino(palabra, idDocumento, i);
                    fwrite(&nuevoTermino, sizeof(termino),1,arch);
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

void generarNombreArchivo(char* nombreArch[20], int id)
{
    char idString[2];
    char cabecera[20] = "texto";
    char* extension = ".txt";
    sprintf(idString, "%i", id);
    strcat(cabecera, idString);
    strcat(cabecera, extension);
    *nombreArch = cabecera;
}


void crearDiccionario(int idMaxima)
{
    char* nombreArchivo, *stringTotal;
    int indice = 1; //inicia en 1 ya que no hay texto0
    while(indice <= idMaxima)
    {
        generarNombreArchivo(&nombreArchivo, indice);
        cargarTextoAString(nombreArchivo, &stringTotal);

        separarPalabras(stringTotal, indice);
        free(stringTotal);
        indice++;
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


int main()
{
    ///troubleshooting
    char * stringTotal;

    cargarTextoAString("texto1.txt", &stringTotal);
    separarPalabras(stringTotal, 1);
    free(stringTotal);

    cargarTextoAString("texto2.txt", &stringTotal);
    separarPalabras(stringTotal, 2);
    free(stringTotal);

    //crearDiccionario(2);
    mostrarDiccionario();

    return 0;
}
