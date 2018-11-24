#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "file_system.hh"

using namespace std;

int largoDireccion(const char *direccion) {
  int cont = 0;
  char *copiaDireccion = new char[strlen(direccion) + 1];
  strcpy(copiaDireccion, direccion);
  char *valor = strtok(copiaDireccion, "/");
  while (valor != NULL) {
    valor = strtok(NULL, "/");
    cont++;
  }
  return cont;
}

void parsearDireccion(string *strArray, int largo, const char *valores) {
  int indice = 0;
  char *copiaValores = new char[strlen(valores) + 1];
  strcpy(copiaValores, valores);
  char *valor;
  valor = strtok(copiaValores, "/");
  while ((valor != NULL) && (indice <= largo)) {
    strArray[indice] = valor;
    valor = strtok(NULL, "/");
    indice++;
  }
}

int *obtenerPosBloques(Inodo inodo) {
  return inodo.posBloque;
}

bool encontreNombre(char *datos, string strArray) {
  bool encontre = false;
  char *copiaDatos = new char[strlen(datos) + 1];
  strcpy(copiaDatos, datos);
  char *valor;
  valor = strtok(copiaDatos, ":");
  while (valor != NULL && !encontre) {
    if (strcmp(valor, strArray.c_str()) == 0) {
      encontre = true;
    } else {
      valor = strtok(NULL, ":");
    }
  }
  return encontre;
}

bool existeDirectorio(Disco disco, string strArray, int *posBloques) {
  bool existe = false;
  int i = 0;
  while (i < 4 && !existe) {
    if (encontreNombre(disco.bloques[posBloques[i]].datos, strArray)) {
      existe = true;
    } else {
      i++;
    }
  }
  return existe;
}

int obtenerInodo(Disco disco, string strArray, int *posBloques) {
  int i = 0;
  while (i < 4) {
    bool encontre = false;
    char *copiaDatos = new char[strlen(disco.bloques[posBloques[i]].datos) + 1];
    strcpy(copiaDatos, disco.bloques[posBloques[i]].datos);
    char *valor;
    valor = strtok(copiaDatos, ":");
    
    while (valor != NULL && !encontre) {
      if (strcmp(valor, strArray.c_str()) == 0) {
        encontre = true;
      } else {
        valor = strtok(NULL, ":");
      }
    }
    return encontre;
  }

}

void crearDirectorio(Disco disco, string *strArray, int largo) {
  bool existePath = true;
  bool existeDir = false;
  int inodo = 0;
  int i = 1;

  while (i < largo && existePath && !existeDir) {
    int *posBloques = obtenerPosBloques(disco.inodos[inodo]);

    if (i == largo - 1) {
      if (existeDirectorio(disco, strArray[i], posBloques)) {
        existeDir = true;
      }
    } else {
      if (existeDirectorio(disco, strArray[i], posBloques)) {
        inodo = obtenerInodo(disco, strArray[i], posBloques);
        i++;
      } else {
        existePath = false;
      }
    }
  }

}
