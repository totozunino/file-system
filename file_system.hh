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

// Retorna el largo del path
int largoDireccion(const char *direccion);

// Parsea el path y lo guarda en strArray
void parsearDireccion(string *strArray, int largo, const char *valores);

// Retorna un array con las posiciones de los bloques
int *obtenerPosBloques(Inodo inodo);

// Retorna true si encontro el directorio en los datos
bool encontreNombre(char *datos, string nombreDir);

// Retorna true si existe el directorio
bool existeDirectorio(Disco disco, string nombreDir, int *posBloques);

// Retorna el inodo segun el nombre del directorio
int obtenerInodo(Disco disco, string nombreDir, int *posBloques);

// Retorna el primer inodo libre que encuentre
string obtenerInodoLibre(Disco disco);

// Retorna el primer bloque libre que encuentre
int obtenerBloqueLibre(Disco disco);

// Crea el directorio
void crearDirectorio(Disco *disco, string nombreDir, int *posBloques);

// Crea el directorio
void checkDirectorio(Disco *disco, string *strArray, int largo);

void imprimirBloque(Disco *disco);

#endif
