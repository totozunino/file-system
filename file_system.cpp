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

bool encontreNombre(char *datos, string nombreDir) {
  bool encontre = false;
  char *copiaDatos = new char[strlen(datos) + 1];
  strcpy(copiaDatos, datos);
  char *valor;
  valor = strtok(copiaDatos, ":");
  while (valor != NULL && !encontre) {
    if (strcmp(valor, nombreDir.c_str()) == 0) {
      encontre = true;
    } else {
      valor = strtok(NULL, ":");
    }
  }
  return encontre;
}

bool existeDirectorio(Disco disco, string nombreDir, int *posBloques) {
  bool existe = false;
  int i = 0;
  while (i < 4 && !existe) {
    if (encontreNombre(disco.bloques[posBloques[i]].datos, nombreDir)) {
      existe = true;
    } else {
      i++;
    }
  }
  return existe;
}

int obtenerInodo(Disco disco, string nombreDir, int *posBloques) {
  int i = 0;
  bool encontre = false;
  int inodo;
  while (i < 4 && !encontre) {
    char *copiaDatos = new char[strlen(disco.bloques[posBloques[i]].datos) + 1];
    strcpy(copiaDatos, disco.bloques[posBloques[i]].datos);
    char *valor;
    valor = strtok(copiaDatos, ":");
    string inodoStr = "-1";
    while (valor != NULL && inodoStr == "-1") {
      if (strcmp(valor, nombreDir.c_str()) == 0) {
        valor = strtok(NULL, ":");
        inodoStr = valor;
        inodo = stoi(inodoStr);
        encontre = true;
      } else {
        valor = strtok(NULL, ":");
      }
    }
    i++;
  }
  return inodo;
}

string obtenerInodoLibre(Disco disco) {
  bool encontre = false;
  int i = 0;
  string inodo;
  while (i < 1024 && !encontre) {
    if (disco.inodos[i].ocupado == false) {
      encontre = true;
      inodo = to_string(i);
    } else {
      i++;
    }
  }
  if (encontre == false) {
    inodo = to_string(-1);
    return inodo;
  } else {
    return inodo;
  }
}

int obtenerBloqueLibre(Disco disco) {
  int i = 0;
  bool encontre = false;
  int bloque = -1;
  while (i < 1024 && !encontre) {
    if (disco.bloques[i].ocupado = false) {
      bloque = i;
      encontre = true;
    } else {
      i++;
    }
  }
  return bloque;
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
        cout << "El directorio ya existe" << endl;
      } else {
        int j = 0;
        bool creado = false;
        while (j < 4 && !creado) {
          string directorio = strArray[i];
          directorio += ":";
          if (obtenerInodoLibre(disco) != "-1") {
            directorio += obtenerInodoLibre(disco);
            directorio += ":";
            if ((512 - strlen(disco.bloques[posBloques[j]].datos)) > strlen(directorio.c_str())) {
              strcat(disco.bloques[posBloques[i]].datos, directorio.c_str());
              disco.inodos[obtenerInodo(disco, strArray[i], posBloques)].ocupado = true;
              disco.inodos[obtenerInodo(disco, strArray[i], posBloques)].esDIR = true;
              int bloque = obtenerBloqueLibre(disco);
              if (bloque != -1) {
                disco.inodos[obtenerInodo(disco, strArray[i], posBloques)].posBloque[bloque] = bloque;
                disco.bloques[bloque].ocupado = true;
                cout << "El directorio " << strArray[i] << "se creo correctamente" << endl;
              } else {
                cout << "No hay suficientes bloques en el sistema de archivos" << endl;
              }
            }

          } else {
            cout << "No hay suficientes inodos en el sistema de archivos" << endl;
          }
        }
      }
    } else {
      if (existeDirectorio(disco, strArray[i], posBloques)) {
        inodo = obtenerInodo(disco, strArray[i], posBloques);
        i++;
      } else {
        existePath = false;
        cout << "El path es incorrecto" << endl;
      }
    }
  }
}
