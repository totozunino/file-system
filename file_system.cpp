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

bool checkNombreDisco(char *nombreDisco) {
  bool correcto = true;
  string s = nombreDisco;
  if (s[0] == '.') {
    correcto = false;
  }
  size_t i = 1;
  while (i < s.length() && correcto) {
    if (s[i] == '/' || s[i] == '.') {
      correcto = false;
    }
    i++;
  }
  return correcto;
}

int cantidadBloques(int *posBloques) {
  int cant = 0;
  for (int i = 0; i < 4; i++) {
    if (posBloques[i] != -1) {
      cant++;
    }
  }
  return cant;
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

bool existeDirectorio(Disco *disco, string nombreDir, int *posBloques) {
  bool existe = false;
  int i = 0;
  while (i < cantidadBloques(posBloques) && !existe) {
    if (encontreNombre(disco->bloques[posBloques[i]].datos, nombreDir)) {
      existe = true;
    } else {
      i++;
    }
  }
  return existe;
}

int obtenerInodo(Disco *disco, string nombreDir, int *posBloques) {
  int i = 0;
  bool encontre = false;
  int inodo = 0;
  while (i < cantidadBloques(posBloques) && !encontre) {
    char *copiaDatos = new char[strlen(disco->bloques[posBloques[i]].datos) + 1];
    strcpy(copiaDatos, disco->bloques[posBloques[i]].datos);
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

int obtenerInodoLibre(Disco *disco) {
  bool encontre = false;
  int i = 0;
  while (i < 1024 && !encontre) {
    if (!disco->inodos[i].ocupado) {
      encontre = true;
    } else {
      i++;
    }
  }
  if (!encontre) {
    return -1;
  } else {
    return i;
  }
}

int obtenerBloqueLibre(Disco *disco) {
  int i = 0;
  bool encontre = false;
  int bloque = -1;
  while (i < 1024 && !encontre) {
    if (!disco->bloques[i].ocupado) {
      bloque = i;
      encontre = true;
    } else {
      i++;
    }
  }
  return bloque;
}

bool checkNombreDir(string nombreDir) {
  return nombreDir.length() <= 8;
}

void crearDirectorio(Disco *disco, string nombreDir, int *posBloques) {
  int i = 0;
  bool creado = false;
  bool asigneBloque = false;
  while (i < cantidadBloques(posBloques) && !creado) {
    string directorio = nombreDir;
    directorio += ":";
    if (obtenerInodoLibre(disco) != -1) {
      string datos = disco->bloques[posBloques[i]].datos;
      size_t tamDatos = 512;
      size_t tamRequerido = strlen(directorio.c_str()) + datos.length();
      if (tamDatos > tamRequerido) {
        int inodo = obtenerInodoLibre(disco);
        directorio += to_string(inodo);
        directorio += ":";
        strcat(disco->bloques[posBloques[i]].datos, directorio.c_str());
        disco->inodos[inodo].ocupado = true;
        disco->inodos[inodo].esDIR = true;
        int bloque = obtenerBloqueLibre(disco);
        if (bloque != -1) {
          disco->inodos[inodo].posBloque[0] = bloque;
          disco->bloques[bloque].ocupado = true;
          creado = true;
          cout << "El directorio " << nombreDir << " se creo correctamente" << endl;
        } else {
          creado = true;
          cout << "No hay suficientes bloques en el sistema de archivos" << endl;
        }
      } else {
        if (i < 3 && !asigneBloque) {
          if (cantidadBloques(posBloques) < 4) {
            int bloque = obtenerBloqueLibre(disco);
            disco->inodos[obtenerInodo(disco, nombreDir, posBloques)].posBloque[cantidadBloques(posBloques)] = bloque;
            disco->bloques[bloque].ocupado = true;
            posBloques = disco->inodos[obtenerInodo(disco, nombreDir, posBloques)].posBloque;
            asigneBloque = true;
            i++;
          } else {
            i++;
            if (i == 3) {
              asigneBloque = true;
            }
          }
        } else if (asigneBloque && i == 3) {
          creado = true;
          cout << "No hay espacio suficiente para poder crear " << nombreDir << endl;
        } else {
          i++;
        }
      }
    } else {
      creado = true;
      cout << "No hay suficientes inodos en el sistema de archivos" << endl;
    }
  }
}

void checkDirectorio(Disco *disco, string *strArray, int largo) {
  bool existePath = true;
  bool existeDir = false;
  bool creado = false;
  int inodo = 0;
  int i = 1;
  while (i < largo && existePath && !existeDir && !creado) {
    int *posBloques = disco->inodos[inodo].posBloque;
    if (i == largo - 1) {
      if (existeDirectorio(disco, strArray[i], posBloques)) {
        existeDir = true;
        cout << "El directorio ya existe" << endl;
      } else {
        crearDirectorio(disco, strArray[i], posBloques);
        creado = true;
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

void mostrarDir(Disco *disco, string nombreDir, int *posBloques) {
  int inodo = obtenerInodo(disco, nombreDir, posBloques);
  int *bloques = disco->inodos[inodo].posBloque;
  int i = 0;
  while (i < cantidadBloques(bloques)) {
    char *copiaDatos = new char[strlen(disco->bloques[bloques[i]].datos + 1)];
    strcpy(copiaDatos, disco->bloques[bloques[i]].datos);
    char *valor;
    valor = strtok(copiaDatos, ":");
    string directorio;
    string inodo;
    if (valor != NULL) {
      directorio = valor;
    }
    bool primera = true;
    while (valor != NULL) {
      if (primera) {
        valor = strtok(NULL, ":");
        inodo = valor;
        primera = false;
      } else {
        valor = strtok(NULL, ":");
        if (valor != NULL) {
          directorio = valor;
          valor = strtok(NULL, ":");
          if (valor != NULL) {
            inodo = valor;
          }
        }
      }
      if (valor != NULL) {
        if (disco->inodos[stoi(inodo)].esDIR) {
          cout << "d " << directorio << endl;
        } else {
          cout << "- " << directorio << endl;
        }
      }
    }
    i++;
  }
}

// hacer las funciones en 1 sola (exiteRuta, checkDirectorio)
void existeRuta(Disco *disco, string *strArray, int largo) {
  bool existeRuta = true;
  bool existeDir = true;
  bool mostrado = false;
  int inodo = 0;
  int i = 1;
  while (i < largo && existeRuta && existeDir && !mostrado) {
    int *posBloques = disco->inodos[inodo].posBloque;
    if (i == largo - 1) {
      if (existeDirectorio(disco, strArray[i], posBloques)) {
        mostrarDir(disco, strArray[i], posBloques);
        mostrado = true;
      } else {
        cout << "El directorio " << strArray[i] << " no existe" << endl;
        existeDir = false;
      }
    } else {
      if (existeDirectorio(disco, strArray[i], posBloques)) {
        inodo = obtenerInodo(disco, strArray[i], posBloques);
        i++;
      } else {
        existeRuta = false;
        cout << "La ruta ingresada es incorrecta" << endl;
      }
    }
  }
}

void imprimirBloque(Disco *disco) {
  int cantBlo = cantidadBloques(disco->inodos[0].posBloque);
  for (int i = 0; i < cantBlo; i++) {
    int blocke = disco->inodos[1].posBloque[i];
    cout << disco->bloques[blocke].datos << endl;
  }
}
