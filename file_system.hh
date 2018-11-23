#ifndef OBLIGATORIO_FILESYSTEM_HH
#define OBLIGATORIO_FILESYSTEM_HH

typedef struct _strBloque *Bloque;

typedef struct _strInodo *Inodo;

typedef struct _strDisco *Disco;

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

// Crea el disco
void crearDisco(Disco disco);

#endif
