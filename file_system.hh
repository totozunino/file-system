#ifndef OBLIGATORIO_FILESYSTEM_HH
#define OBLIGATORIO_FILESYSTEM_HH

#include <iostream>
#include <string.h>

using namespace std;

typedef struct _strBloque Bloque;

typedef struct _strInodo Inodo;

typedef struct _strDisco Disco;

struct _strBloque {
    bool ocupado;
    char datos[512];
};

struct _strInodo {
    bool ocupado;
    bool esDIR;
    int posBloque[4];
};

struct _strDisco {
    Inodo inodos[1024];
    Bloque bloques[1024];
};

// Retorna el largo de la ruta
int largoRuta(const char *direccion);

// Parsea la ruta y la guarda en strArray
void parsearRuta(string *strArray, int largo, const char *valores);

// Checkea si existe la ruta
bool existeRuta(Disco *disco, string *strArray, int largo, int &inodoFinal);

// Retorna true si encontro el directorio en los datos
bool encontreNombre(char *datos, string nombreDir);

// Retorna true si existe el directorio
bool existeDirectorio(Disco *disco, string nombreDir, int *posBloques);

// Retorna la cantidad de bloques
int cantidadBloques(int *posBloques);

// Retorna el inodo segun el nombre del directorio
int obtenerInodo(Disco *disco, string nombreDir, int *posBloques);

// Retorna el primer inodo libre que encuentre
int obtenerInodoLibre(Disco *disco);

// Retorna el primer bloque libre que encuentre
int obtenerBloqueLibre(Disco *disco);

// Crea el directorio
void crearArchivo(Disco *disco, string nombreDir, int *posBloques, bool esDir);

// Imprime el contenido de un archivo
void imprimirArchivo(Disco *disco, string nombreArchivo, int *posBloques);

// Escribo  al final del archivo el texto ingresado
void escribirArchivo(Disco *disco, string nombreArchivo, int *posBloques, char *datos);

// Borra una directorio
void borrarDirectorio(Disco *disco, string nombreDirectorio, int *posBloques);

// Borra un archivo
void borrarArchivo(Disco *disco, string nombreArchivo, int *posBloques);
// void imprimirBloque(Disco *disco);

// Retorna true si el nombre del disco es correcto
bool checkNombreDisco(char *nombreDisco);

// Muestra el contenido de un directorio
void mostrarDir(Disco *disco, string nombreDir, int *posBloques);

// Retorna true si el nombre del archivo es correcto
bool checkNombreArchivo(string nombreArchivo);

#endif
