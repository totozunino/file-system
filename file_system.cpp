#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "file_system.hh"

struct _strBloque{
    bool ocupado;
    char datos[512];
};

struct _strInodo{
    bool ocupado;
    bool esDIR;
    Bloque bloques[4];
};

struct _strFileSystem{
    Inodo inodos[1024];
    Bloque bloques[1024];
};
