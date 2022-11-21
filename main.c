#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define diccionario "diccionario.bin"
//!-----------------------------------ESTRUCTURA DEL DICCIONARIO----------------------------------------------------

typedef struct
{

    char palabra[20];
    int idDOC;
    int pos; //incrementa palabra por palabra, y no letra por letra

} Termino;

//!-----------------------------------ESTRUCTURA DEL ARBOL----------------------------------------------------

typedef struct nodoT
{
    int idDOC;
    int pos;
    struct nodoT* sig;

} nodoT;

typedef struct nodoA
{
    char palabra[20];
    int frecuencia; //representa la cantidad de nodos de la lista
    nodoT* ocurrencias; //ordenada por idDOC, luego por pos
    struct nodoA* der;
    struct nodoA* izq;
} nodoA;

//!-----------------------------------ESTRUCTURA DE LA LISTA DE IDS----------------------------------------------------

typedef struct nodoId
{

    int id;
    struct nodoId* sig;

} nodoId;

//!-----------------------------------FUNCIONES BASE----------------------------------------------------

nodoT* crearNodoT(Termino dato)
{
    nodoT* nuevo=(nodoT*)malloc(sizeof(nodoT));
    nuevo->idDOC=dato.idDOC;
    nuevo->pos=dato.pos;
    nuevo->sig=NULL;
    return nuevo;
}

nodoA* crearNodoA(Termino dato)
{
    nodoA* nuevo=(nodoA*)malloc(sizeof(nodoA));
    strcpy(nuevo->palabra,dato.palabra);
    nuevo->frecuencia=1;
    nuevo->ocurrencias=crearNodoT(dato);
    nuevo->izq=NULL;
    nuevo->der=NULL;
    return nuevo;
}

void mostrarLista(nodoT* lista)
{
    while(lista)
    {

        printf("\nId: %d",lista->idDOC);
        printf("\nPos: %d",lista->pos);

        printf("\n.....................\n");

        lista=lista->sig;
    }
}

void mostrarArbol(nodoA* arbol)
{
    if(arbol)
    {
        mostrarArbol(arbol->izq);
        printf("\nPalabra: %s",arbol->palabra);
        printf("\nFrecuencia: %d",arbol->frecuencia);
        mostrarLista(arbol->ocurrencias);
        printf("\n\n-------------------\n\n");
        mostrarArbol(arbol->der);
    }
}

nodoId* crearNodoId(int dato)
{
    nodoId* nuevo=(nodoId*)malloc(sizeof(nodoId));
    nuevo->id=dato;
    nuevo->sig=NULL;
    return nuevo;
}

void mostrarListaId(nodoId* lista)
{
    while(lista){
        printf("\n%d ",lista->id);
        lista=lista->sig;
    }
}

//!-----------------------------------FUNCIONES PARTE ARBOL----------------------------------------------------

void insertarNuevoTermino(nodoA** arbol,Termino dato)
{
    if(*arbol)
    {
        if(strcmp(dato.palabra,(*arbol)->palabra)>0)
        {
            insertarNuevoTermino(&(*arbol)->der,dato);
        }
        else
        {
            insertarNuevoTermino(&(*arbol)->izq,dato);
        }
    }
    else
    {

        (*arbol)=crearNodoA(dato);
    }

}

void insertarNuevaOcurrencia(nodoT** lista,Termino dato)//primer criterio de orden ID, segundo POS
{
    nodoT* nuevo=crearNodoT(dato);
    nodoT* ant,* seg;

    if(*lista)
    {

        if((*lista)->idDOC > dato.idDOC || ( (*lista)->idDOC == dato.idDOC && (*lista)->pos > dato.pos))
        {

            nuevo->sig=(*lista);
            (*lista)=nuevo;

        }
        else
        {

            ant=(*lista);
            seg=(*lista)->sig;

            while(seg && ( (seg->idDOC < dato.idDOC) || (seg->idDOC == dato.idDOC && seg->pos < dato.pos) ) )
            {

                ant=seg;
                seg=seg->sig;
            }

            ant->sig=nuevo;
            nuevo->sig=seg;
        }
    }
}

void insertarTerminoExistente(nodoA** arbol,Termino dato)
{
    //modificacion de la frecuencia
    (*arbol)->frecuencia+=1;

    //insercion en la lista
    insertarNuevaOcurrencia(&(*arbol)->ocurrencias,dato);
}

void existeTermino(nodoA* arbol,char palabra[],nodoA** encontrado)
{
    if(arbol)
    {

        if(strcmp(arbol->palabra,palabra)!=0)
        {

            if(strcmp(palabra,arbol->palabra)>0)
            {
                existeTermino(arbol->der,palabra,encontrado);
            }
            else
            {
                existeTermino(arbol->izq,palabra,encontrado);
            }
        }
        else
        {
            *encontrado=arbol;
        }
    }
}

void cargarTermino(nodoA** arbol,Termino aux)
{
    //fijarse si ya existe la palabra en el arbol
    nodoA* existencia=NULL;
    existeTermino(*arbol,aux.palabra,&existencia);

    if(existencia)  //si existe se le incrementa frecuencia y ademas agregar un nodo a la lista
    {

        insertarTerminoExistente(&existencia,aux);

    }
    else   //sino se crea un nuevo nodo en el arbol
    {

        insertarNuevoTermino(arbol,aux);
    }
}

void cargarArbol(nodoA** arbol)
{
    Termino aux;
    FILE* arch = fopen(diccionario,"rb");

    if(arch)
    {

        while(fread(&aux,sizeof(Termino),1,arch)>0)
        {

            cargarTermino(arbol,aux);

        }

        fclose(arch);
    }
}

//!-----------------------------------FUNCIONES PARTE USUARIO---------------------------------------------------

/// 1) Buscar todas las apariciones de un término en algún documento (operación or).
/// (Tiene que estar en alguno de los textos)

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

void encontrarPalabra(nodoA* arbol,char palabra[20], nodoT** encontrado)
{
    if(arbol)
    {

        if(strcmpi(arbol->palabra,palabra)!=0)
        {

            if(strcmpi(arbol->palabra,palabra)<0)
            {
                encontrarPalabra(arbol->der,palabra,encontrado);
            }
            else
            {
                encontrarPalabra(arbol->izq,palabra,encontrado);
            }

        }
        else
        {
            *encontrado = arbol->ocurrencias;
        }
    }
    else
    {
        *encontrado = NULL;
    }
}

void buscarApariciones(nodoA* arbol,char palabra[20])
{
    nodoT* encontrado;
    encontrarPalabra(arbol, palabra, &encontrado);
    if(encontrado ==NULL)
    {
        printf("Sin resultados.\n");
    }
    else
    {
        printf("\n\tLISTA DE IDS");
        mostrarLista(encontrado);
    }
}


/// 2) Buscar todas las apariciones de un término en un documento y otros  (operacion and).
/// (Tiene que sí o sí estar en todos los textos que el usuario le indica)

int buscarIdXDocumento(nodoT* lista,nodoId* listaId)
{
    int flag=1;

    if(lista && listaId)
    {
        while(listaId && flag==1)//id ingresados por el usuario
        {
            while(lista && (lista->idDOC < listaId->id)) // lista del arbol
            {
                lista=lista->sig;
            }

            if(lista && (lista->idDOC == listaId->id)) //si se encuentra el id se pasa al siguiente
            {
                listaId=listaId->sig;
            }
            else //si no se encontro el id entonces se levanta la bandera
            {
                flag=0;
            }
        }
    }
    return flag;
}

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
    }else{

        (*lista)=nuevo;
    }
}

void buscarAparicionesXDocumento(nodoA* arbol,char palabra[20])
{
    int idIngresado=0;
    char control='s';
    nodoT* encontrado;
    int coincidencias=0;

    nodoId* lista=NULL;

    //primero encontrar la palabra-----------------------------
    encontrarPalabra(arbol, palabra, &encontrado);

    if(encontrado)
    {
        //segundo cargar una lista con los id que indica el usuario y ordenarla-------------------
        printf("Ingrese los id de los documentos donde desea realizar la busqueda: ");

        while(control=='s')
        {
            printf("Id: ");
            fflush(stdin);
            scanf("%d",&idIngresado);

            insertarId(&lista,idIngresado);

            printf("Seleccionar otro id? s/n: ");
            fflush(stdin);
            scanf("%c",&control);
        }

        //tercero buscar los id en la lista----------------------------------------

        coincidencias=buscarIdXDocumento(encontrado,lista);

        if(coincidencias)
        {
            printf("\n\tLISTA DE IDS");
            mostrarListaId(lista);
            printf("\nPalabras encontradas en todos los documentos");
        }
        else
        {
            printf("\nPalabra no encontrada en todos los documentos.");
        }
    }
    else
    {
        printf("Palabra no encontrada.");
    }
}

/// 3)Buscar la aparición de más de un término en el mismo documento.

int main()
{
    nodoA* arbol=NULL;
    cargarArbol(&arbol);

    mostrarArbol(arbol);
    printf("---------BUSQUEDA-------\n");
    //buscarApariciones(arbol, "salame");
    //buscarAparicionesXDocumento(arbol,"salame");

    return 0;
}
