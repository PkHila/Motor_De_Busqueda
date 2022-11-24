#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "motorDeBusqueda.h"

#include "funcionesUsuarios.h"

#define diccionario "diccionario.bin"

//!-----------------------------------FUNCIONES BASE----------------------------------------------------

nodoId* crearNodoId(int dato)
{
    nodoId* nuevo=(nodoId*)malloc(sizeof(nodoId));
    nuevo->id=dato;
    nuevo->sig=NULL;
    return nuevo;
}

//?
void mostrarListaId(nodoId* lista)
{
    while(lista){
        printf("\n%d ",lista->id);
        lista=lista->sig;
    }
}
//?

nodoPalabra* crearNodoPalabra(char dato[20])
{
    nodoPalabra* nuevo=(nodoPalabra*)malloc(sizeof(nodoPalabra));
    strcpy(nuevo->palabra,dato);
    nuevo->sig=NULL;
    return nuevo;
}

void mostrarListaPlabras(nodoPalabra* lista)
{
    while(lista)
    {
        printf("\n%s ",lista->palabra);
        lista=lista->sig;
    }
}

//!-----------------------------------FUNCIONES CARGA DE DATOS----------------------------------------------------

void insertarId(nodoId** lista,int dato)//orden ascendente
{
    nodoId* nuevo=crearNodoId(dato);
    nodoId* ant, *seg;

    if(*lista)
    {

        if((*lista)->id > dato)
        {

            nuevo->sig=(*lista);
            (*lista)=nuevo;

        }
        else
        {

            ant=(*lista);
            seg=(*lista)->sig;

            while(seg && seg->id < dato)
            {
                ant=seg;
                seg=seg->sig;
            }

            ant->sig=nuevo;
            nuevo->sig=seg;
        }
    }
    else
    {

        (*lista)=nuevo;
    }
}

void cargarIds(nodoId** lista)
{
    int id=0;
    char control='s';

    while(control=='s')
    {
        printf("\nIngrese los id donde desea realizar la busqueda: ");
        fflush(stdin);
        scanf("%d",&id);

        //!VERIFICACION DE ID VALIDO //! HILA CANT_TEXTOS LIBRERIA DICCIONARIO

        insertarId(lista,id);

        printf("\nCargar otro id? s/n: ");
        fflush(stdin);
        scanf("%c",&control);

        system("cls");
    }
}

void insertarPalabra(nodoPalabra** lista,char palabra[])
{
    nodoPalabra* nuevo=crearNodoPalabra(palabra);
    nuevo->sig=(*lista);
    (*lista)=nuevo;
}

int verificarPalabra(nodoA* arbol,char palabra[])
{
    int encontrado=0;

    if(arbol)
    {

        if(strcmpi(arbol->palabra,palabra)!=0)
        {
            if(strcmpi(palabra,arbol->palabra)>0)
            {
                encontrado=verificarPalabra(arbol->der,palabra);
            }
            else
            {
                encontrado=verificarPalabra(arbol->izq,palabra);
            }
        }
        else
        {
            encontrado=1;
        }
    }

    return encontrado;
}

void cargarPalabras(nodoA* arbol,nodoPalabra** lista)
{
    char palabra[20]= {0};
    char control='s';
    int validacion=0;

    while(control=='s')
    {
        printf("Ingrese la palabra que desea buscar: ");
        fflush(stdin);
        gets(palabra);

        validacion=verificarPalabra(arbol,palabra);

        if(validacion)
        {
            insertarPalabra(lista,palabra);

            printf("\nCargar otra palabra? s/n: ");
            fflush(stdin);
            scanf("%c",&control);

        }
        else
        {
            printf("\nPalabra ingresada no valida. Vuelva a ingresar.\n");
            system("pause");
        }

        system("cls");
    }
}

//!-----------------------------------FUNCIONES DE BUSQUEDA----------------------------------------------------

/// 1) BUSCAR APARICIONES EN ALGUNOS DOCS

void buscarAparicionesEnAlgunosDocs(nodoA* arbol,char palabra[20],nodoT** apariciones)
{
    //encontrar la palabra
    nodoA* palabraEncontrada;
    existeTermino(arbol,palabra,&palabraEncontrada);

    //retorno resultado
    (*apariciones)=palabraEncontrada->ocurrencias;
}

/// 2) BUSCAR APARICIONES EN TODOS LOS DOS

int coincideId(nodoT* lista,int idBuscado,nodoT** apariciones)
{
    int encontrado=0;

    if(lista)
    {

        while(lista && lista->idDOC < idBuscado)
        {
            lista=lista->sig;
        }

        if(lista && lista->idDOC == idBuscado)
        {
            encontrado=1;

            while(lista && lista->idDOC == idBuscado)
            {
                insertarNuevaOcurrencia(apariciones,lista->idDOC,lista->pos);
                lista=lista->sig;
            }
        }
    }

    return encontrado;
}

int buscarAparicionesEnTodosDocs(nodoA* arbol,char palabra[],nodoId* idBuscado,nodoT** apariciones)
{
    int coincide=1;
    nodoT* aux=NULL;

    //encontrar la palabra
    nodoA* palabraEncontrada;
    existeTermino(arbol,palabra,&palabraEncontrada);

    //filtro de apariciones
    while(idBuscado && coincide==1)
    {
        coincide=coincideId(palabraEncontrada->ocurrencias,idBuscado->id,&aux); //filtro de coincidencia

        if(coincide)
        {
            idBuscado=idBuscado->sig; //avanza al sig id solo si el anterior ya se encontro
        }
    }

    if(coincide)
    {
        (*apariciones)=aux;
        return 1;
    }
    else
    {
        return 0;
    }
}

/// 3)Buscar la aparicion de mas de un termino en el mismo documento.

void buscarVariasAparicionesEnXDoc(nodoA* arbol,nodoPalabra* palabras,int id,nodoT** ocurrencias)
{
    int coincide=0;
    nodoA* palabraEncontrada;

    while(palabras)
    {
        existeTermino(arbol,palabras->palabra,&palabraEncontrada);

        coincide=coincideId(palabraEncontrada->ocurrencias,id,ocurrencias); //filtro de coincidencia

        //se podria avisar cuales si y cuales no con la var coincide

        palabras=palabras->sig;
    }
}

/// 4. Buscar una frase completa (las palabras deben estar contiguas en alguno de los documentos).

/// 5. Buscar la palabra de más frecuencia que aparece en un doc

int sumarApariciones(nodoT* lista,int id)
{
    int cant=0;

    if(lista)
    {

        while(lista && lista->idDOC < id)
        {
            lista=lista->sig;
        }

        while(lista && lista->idDOC == id)
        {
            cant+=1;
            lista=lista->sig;
        }
    }

    return cant;
}

void encontrarPalabraMasFrecuente(nodoA* arbol,int id,char palabra[],int* maxApariciones)
{
    int cant=0;

    if(arbol)
    {

        cant=sumarApariciones(arbol->ocurrencias,id);

        if(cant > *maxApariciones)
        {
            *maxApariciones=cant;
            strcpy(palabra,arbol->palabra);
        }

        encontrarPalabraMasFrecuente(arbol->izq,id,palabra,maxApariciones);
        encontrarPalabraMasFrecuente(arbol->der,id,palabra,maxApariciones);
    }
}

void buscarPalabraMasFrecuente(nodoA* arbol,int id,nodoT** apariciones)
{
    char palabra[20]= {0};
    nodoA* palabraEncontrada;
    int maxApariciones=-1;

    encontrarPalabraMasFrecuente(arbol,id,palabra,&maxApariciones);

    existeTermino(arbol,palabra,&palabraEncontrada);

    coincideId(palabraEncontrada->ocurrencias,id,apariciones);
}

/// 6. Utilizar la distancia de levenshtein en el ingreso de una palabra y sugerir similares a partir de una distancia <= 3.

//!-----------------------------------FUNCIONES DE MOSTRAR RESULTADOS----------------------------------------------------

void mostrarPalabra(Termino temp,nodoT* lista)
{
    if(lista){

        while(lista && lista->idDOC < temp.idDOC){

            lista=lista->sig;
        }

        if(lista && lista->idDOC == temp.idDOC){

            while(lista && lista->idDOC == temp.idDOC && lista->pos < temp.pos){
                lista=lista->sig;
            }

            if(lista && lista->idDOC == temp.idDOC && lista->pos == temp.pos){
                printf("<< %s >> ",temp.palabra);

            }else{
                printf("%s ",temp.palabra);
            }

        }
    }
}

void mostrarDocumento(nodoT* lista)
{
    int i=0;
    Termino temp;
    FILE* arch=fopen(diccionario,"rb");

    if(arch)
    {
        while(fread(&temp,sizeof(Termino),1,arch)>0)
        {
            mostrarPalabra(temp,lista);
            i+=1;

             if(i==13){
                 printf("\n");
                 i=0;
             }
        }

        fclose(arch);
    }
}

