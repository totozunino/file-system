#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "file_system.hh"

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

Disco crearDisco() {
  Disco disco = new _strDisco;
  for (int i = 0; i < 1024; i++) {
    disco->bloques[i] = new _strBloque;
    disco->inodos[i] = new _strInodo;
  }
  disco->inodos[0]->esDIR = true;
  disco->inodos[0]->ocupado = true;
  disco->inodos[0]->posBloque[0] = 0;
  disco->bloques[0]->ocupado = true;
  return disco;
}
