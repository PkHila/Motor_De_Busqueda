#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionesUsuarios.h"



//!-----------------------------------FUNCIONES BASE----------------------------------------------------

nodoId* crearNodoId(int dato)
{
    nodoId* nuevo=(nodoId*)malloc(sizeof(nodoId));
    nuevo->id=dato;
    nuevo->sig=NULL;
    return nuevo;
}

void mostrarListaId(nodoId* lista)
{
    while(lista)
    {
        printf("\n%d ",lista->id);
        lista=lista->sig;
    }
}

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

int cargarPalabra(nodoA* arbol,char palabra[])
{
    int verificacion=0;

    printf("\nIngrese la palabra que desea buscar: ");
    fflush(stdin);
    gets(palabra);

    verificacion=verificarPalabra(arbol,palabra);

    return verificacion;
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
    }
    else
    {

        (*lista)=nuevo;
    }
}

int cargarIds(nodoId** lista)
{
    int verificacion=1;
    int id=0;
    char control='s';

    while(control=='s')
    {
        printf("\nIngrese los id donde desea realizar la busqueda: ");
        fflush(stdin);
        scanf("%d",&id);

        if(id <= CANT_TEXTOS)
        {
            insertarId(lista,id);

        }
        else
        {

            verificacion=0;
        }

        printf("\nCargar otro id? s/n: ");
        fflush(stdin);
        scanf("%c",&control);
    }
    return verificacion;
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

void cargarPalabras(nodoA* arbol,nodoPalabra** lista,nodoPalabra** noEncontradas)
{
    char palabra[20]= {0};
    char control='s';
    int validacion=0;

    while(control=='s')
    {
        printf("\nIngrese la palabra que desea buscar: ");
        fflush(stdin);
        gets(palabra);

        validacion=verificarPalabra(arbol,palabra);

        if(validacion)
        {
            insertarPalabra(lista,palabra);
        }
        else
        {
            insertarPalabra(noEncontradas,palabra);
        }

        printf("\nCargar otra palabra? s/n: ");
        fflush(stdin);
        scanf("%c",&control);
    }
}

int ingresoIdDoc(int* idDoc)
{
    int verificacion=0;
    int idIngresado=0;

    printf("\nIngrese el id donde desea realizar la busqueda: ");
    fflush(stdin);
    scanf("%d",&idIngresado);

    if(idIngresado <= CANT_TEXTOS)
    {
        *idDoc=idIngresado;
        verificacion=1;
    }

    return verificacion;
}

//!-----------------------------------FUNCIONES DE BUSQUEDA----------------------------------------------------

/// 1) BUSCAR APARICIONES EN ALGUNOS DOCS

void buscarAparicionesEnAlgunosDocs(nodoA* arbol,char palabra[20],nodoT** apariciones)
{
    //encontrar la palabra
    nodoA* palabraEncontrada;
    existeTermino(arbol,palabra,&palabraEncontrada);

    //retorno la sublista con todas las ocurrencias de la palabra
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

        coincide=coincideId(palabraEncontrada->ocurrencias,id,ocurrencias);

        //las palabras que existen pero no estan en ese documento?

        palabras=palabras->sig;
    }
}

/// 4. Buscar una frase completa (las palabras deben estar contiguas en alguno de los documentos).

void ingresarFrase(char* frase)
{
    printf("Ingrese la frase a buscar\n");
    fflush(stdin);
    gets(frase);
}

void separarFrase(nodoA * arbol, char* frase, char arregloPalabras[][20], int* validos)
{
    int indiceFrase = 0;
    int indicePalabra = 0;
    int indiceValidos = 0;
    while(indiceFrase < strlen(frase) && frase[indiceFrase]!='\0')
    {
        if(caracterValido(frase[indiceFrase])==1)
        {
            arregloPalabras[indiceValidos][indicePalabra] = frase[indiceFrase];
            indicePalabra++;
        }
        else
        {
            if(indicePalabra>0)
            {
                indicePalabra = 0;
                indiceValidos++;
            }
        }
        indiceFrase++;
    }

    if(indicePalabra>0)
    {
        indiceValidos++;
    }
    *validos = indiceValidos;

}

void buscarPalabrasContinuas(nodoT** arreglo,int validos, nodoT** apariciones)
{
    int i = 1; //indice de validos
    int continuo = 0;

    while(arreglo[0])
    {
        while(arreglo[0] && arreglo[i] && arreglo[0]->idDOC >= arreglo[i]->idDOC) //recorre mientras que el id de la comparacion sea menor o igual
        {
            if(arreglo[0]->idDOC == arreglo[i]->idDOC)
            {
                    if(arreglo[0]->pos+i == arreglo[i]->pos)
                    {
                        if(i+1<validos) //si el sig indice est� dentro de validos
                        {
                            i++;
                            continuo = 1;
                        }
                        else
                        {
                            insertarNuevaOcurrencia(apariciones,arreglo[0]->idDOC, arreglo[0]->pos);
                            arreglo[0] = arreglo[0]->sig;
                            i = 1;
                        }
                    }
            }
            if(arreglo[i])
            {
                if(continuo == 0)
                {
                    arreglo[i] = arreglo[i]->sig;
                }
                else
                {
                    continuo = 0;
                }
            }
        }
        if(arreglo[0])
        arreglo[0] = arreglo[0]->sig;
        i = 1;

    }
}


void buscarFrase(nodoA* arbol, nodoT** apariciones)
{
    ///El usuario ingresa la frase
    char fraseBusqueda[100] = {0};
    ingresarFrase(fraseBusqueda);

    ///Se separa la frase en palabras
    char arregloPalabras[20][20] = {0};
    int validos = 0;
    separarFrase(arbol, fraseBusqueda, arregloPalabras,&validos);

    ///Comprobamos que las palabras existan
    nodoA* encontrado = NULL;
    nodoT* arregloOcurrencias[validos];
    int i = 0;
    int flag = 1;
    while(i < validos && flag ==1)
    {
        ///y vamos guardando sus datos mientras lo hacemos
        existeTermino(arbol, arregloPalabras[i], &encontrado);
        if(!encontrado)
        {
            flag = 0;
        }
        else
        {
            arregloOcurrencias[i] = encontrado->ocurrencias;
            i++;
        }
    }

    ///si las palabras individuales existen, procedemos a ver si son continuas
    if(flag)
    {
        if(validos==1) //si la frase consiste de una sola palabra, usamos la respectiva funcion
        {
            printf("La frase consiste de una sola palabra. Estas son sus apariciones:\n\n");
            nodoT* apariciones;
            buscarAparicionesEnAlgunosDocs(arbol, arregloPalabras[0], &apariciones);
            mostrarLista(apariciones);
        }
        else
        {
            buscarPalabrasContinuas(arregloOcurrencias, validos, apariciones);
        }
    }
    else
    {
        printf("La palabra \"%s\" no se encuentra en ningun texto\n", arregloPalabras[i]);
    }
}
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
    int coincide=0;

    encontrarPalabraMasFrecuente(arbol,id,palabra,&maxApariciones);

    existeTermino(arbol,palabra,&palabraEncontrada);

    coincide=coincideId(palabraEncontrada->ocurrencias,id,apariciones);
}

/// 6. Utilizar la distancia de levenshtein en el ingreso de una palabra y sugerir similares a partir de una distancia <= 3.

int Minimo(int a, int b)
{
    if(a < b) return a;
    return b;
}

int Levenshtein(char *s1,char *s2)
{
    int t1,t2,i,j,*m,costo,res,ancho;

// Calcula tamanios strings
    t1=strlen(s1);
    t2=strlen(s2);

// Verifica que exista algo que comparar
    if (t1==0) return(t2);
    if (t2==0) return(t1);
    ancho=t1+1;

// Reserva matriz con malloc                     m[i,j] = m[j*ancho+i] !!
    m=(int*)malloc(sizeof(int)*(t1+1)*(t2+1));
    if (m==NULL) return(-1); // ERROR!!

// Rellena primera fila y primera columna
    for (i=0; i<=t1; i++) m[i]=i;
    for (j=0; j<=t2; j++) m[j*ancho]=j;

// Recorremos resto de la matriz llenando pesos
    for (i=1; i<=t1; i++) for (j=1; j<=t2; j++)
        {
            if (s1[i-1]==s2[j-1]) costo=0;
            else costo=1;
            m[j*ancho+i]=Minimo(Minimo(m[j*ancho+i-1]+1,     // Eliminacion
                                       m[(j-1)*ancho+i]+1),              // Insercion
                                m[(j-1)*ancho+i-1]+costo);
        }      // Sustitucion

// Devolvemos esquina final de la matriz
    res=m[t2*ancho+t1];
    free(m);
    return(res);
}

void sugerirSimilares(nodoA* arbol,char palabra[])
{
    if(arbol){

        int dist=Levenshtein(arbol->palabra,palabra);

        if(dist <= 3){
            printf("\n- %s",arbol->palabra);
        }

        sugerirSimilares(arbol->izq,palabra);
        sugerirSimilares(arbol->der,palabra);
    }
}

//!-----------------------------------FUNCIONES DE MOSTRAR RESULTADOS----------------------------------------------------

void mostrarPalabrasNoEncontradas(nodoPalabra* lista)
{
    if(lista)
    {
        printf("\nPalabras no encontradas en ningun documento: \n");

        while(lista)
        {
            printf("- %s\n",lista->palabra);
            lista=lista->sig;
        }
    }
}

int mostrarPalabra(Termino temp,nodoT* lista,int avisoImpresion)
{
    int flag=0;

    if(lista)
    {
        while(lista && lista->idDOC < temp.idDOC)
        {

            lista=lista->sig;
        }

        if(lista && lista->idDOC == temp.idDOC)
        {

            while(lista && lista->idDOC == temp.idDOC && lista->pos < temp.pos)
            {
                lista=lista->sig;
            }

            flag=1;
            if(avisoImpresion)
            {
                printf("\n\n\t\t-------------DOC %d---------------\n\n",temp.idDOC);
            }

            if(lista && lista->idDOC == temp.idDOC && lista->pos == temp.pos)
            {
                printf("<<  %s  >> ",temp.palabra);

            }
            else
            {
                printf("%s ",temp.palabra);
            }

        }
    }

    return flag;
}

void mostrarDocumento(nodoT* lista,int* docAnt)
{
    int i=0;
    int flag=0;
    int avisoImpresion=0;
    Termino temp;

    if(lista)
    {
        FILE* arch=fopen(diccionario,"rb");

        if(arch)
        {
            while(fread(&temp,sizeof(Termino),1,arch)>0)
            {
                if(temp.idDOC != *docAnt)
                {
                    avisoImpresion=1;

                    *docAnt=temp.idDOC;
                }

                flag=mostrarPalabra(temp,lista,avisoImpresion);
                avisoImpresion=0;

                if(flag)
                {
                    i+=1;
                }

                if(i==13)
                {
                    printf("\n");
                    i=0;
                }
            }

            fclose(arch);
        }
    }
    printf("\n\n");
}
