#ifndef DICCIONARIO_H_INCLUDED
#define DICCIONARIO_H_INCLUDED

#define DICCIONARIO "diccionario.bin"
#define CANT_TEXTOS 10

typedef struct
{

    char palabra[20];
    int idDOC;
    int pos; //incrementa palabra por palabra, y no letra por letra

} termino;

termino crearTermino(char palabra[20], int idDocumento, int pos);
void cargarTextoAString(char* nombreTexto, char** textoCompleto);
int caracterValido(char c);
void separarPalabras(char* textoCompleto, int idDocumento);
char* generarNombreArchivo(int idDocumento);
void crearNuevoDiccionario();
void cargarDiccionario();

#endif // DICCIONARIO_H_INCLUDED
