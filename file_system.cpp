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

int largoRuta(const char *direccion) {
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

void parsearRuta(string *strArray, int largo, const char *valores) {
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

void crearArchivo(Disco *disco, string nombreDir, int *posBloques, bool esDir) {
  int i = 0;
  bool dirCreado = false;
  while (i < cantidadBloques(posBloques) && !dirCreado) {
    string directorio = nombreDir;
    directorio += ":";
    if (obtenerInodoLibre(disco) != -1) {
      int inodo = obtenerInodoLibre(disco);
      directorio += to_string(inodo);
      directorio += ":";
      string datos = disco->bloques[posBloques[i]].datos;
      size_t tamDatos = 512;
      size_t tamRequerido = strlen(directorio.c_str()) + datos.length();
      if (tamDatos > tamRequerido) {
        strcat(disco->bloques[posBloques[i]].datos, directorio.c_str());
        disco->inodos[inodo].ocupado = true;
        disco->inodos[inodo].esDIR = esDir;
        int bloque = obtenerBloqueLibre(disco);
        if (bloque != -1) {
          disco->inodos[inodo].posBloque[0] = bloque;
          disco->bloques[bloque].ocupado = true;
          strcpy(disco->bloques[bloque].datos, "");
          dirCreado = true;
          if (esDir) {
            cout << "El directorio '" << nombreDir << "' se creo correctamente" << endl;
          } else {
            cout << "El archivo '" << nombreDir << "' se creo correctamente" << endl;
          }
        } else {
          dirCreado = true;
          cout << "No hay suficientes bloques en el sistema de archivos" << endl;
        }
      } else {
        if (i != 3) {
          if (i == cantidadBloques(posBloques) - 1) {
            int bloque = obtenerBloqueLibre(disco);
            disco->inodos[obtenerInodo(disco, nombreDir, posBloques)].posBloque[cantidadBloques(posBloques)] = bloque;
            disco->bloques[bloque].ocupado = true;
            strcpy(disco->bloques[bloque].datos, "");
            posBloques = disco->inodos[obtenerInodo(disco, nombreDir, posBloques)].posBloque;
            i++;
          } else {
            i++;
          }
        } else {
          cout << "No hay espacio suficiente para poder crear " << nombreDir << endl;
          dirCreado = true;
        }
      }
    } else {
      dirCreado = true;
      cout << "No hay suficientes inodos en el sistema de archivos" << endl;
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

bool existeRuta(Disco *disco, string *strArray, int largo, int &inodoFinal) {
  bool existeRuta = true;
  int inodo = 0;
  int i = 1;
  while (i < largo - 1 && existeRuta) {
    int *posBloques = disco->inodos[inodo].posBloque;
    if (existeDirectorio(disco, strArray[i], posBloques)) {
      inodo = obtenerInodo(disco, strArray[i], posBloques);
      if (!disco->inodos[inodo].esDIR) {
        existeRuta = false;
      }
      i++;
    } else {
      existeRuta = false;
    }
  }
  inodoFinal = inodo;
  return existeRuta;
}

bool checkNombreArchivo(string nombreArchivo) {
  if (nombreArchivo.length() <= 8) {
    return true;
  } else {
    if (nombreArchivo.length() < 13) {
      return nombreArchivo[8] == '.';
    } else {
      return false;
    }
  }
}

void imprimirArchivo(Disco *disco, string nombreArchivo, int *posBloques) {
  int inodo = obtenerInodo(disco, nombreArchivo, posBloques);
  if (!disco->inodos[inodo].esDIR) {
    int *bloques = disco->inodos[inodo].posBloque;
    int cantBloques = cantidadBloques(bloques);
    for (int i = 0; i < cantBloques; i++) {
      cout << disco->bloques[bloques[i]].datos;
    }
  } else {
    cout << "Error '" << nombreArchivo << "' no es un archivo" << endl;
  }
}

void escribirArchivo(Disco *disco, string nombreArchivo, int *posBloques, char *datos) {
  int inodo = obtenerInodo(disco, nombreArchivo, posBloques);
  if (!disco->inodos[inodo].esDIR) {
    int *bloques = disco->inodos[inodo].posBloque;
    int cantBloques = cantidadBloques(bloques);
    int cantDatos = strlen(datos);
    int cantEscritos = 0;
    while (cantDatos != cantEscritos) {
      // to do escribir archivo function
      //size_t
      //tamRequerido = cantDatos + disco
      //if (cantDatos <)
    }
  } else {
    cout << "Error '" << nombreArchivo << "' no es un archivo" << endl;
  }
}

bool esVacio(Disco *disco, int *posBloques) {
  bool esVacio = true;
  if (disco->bloques[posBloques[0]].datos[0] != '\377' && disco->bloques[posBloques[0]].datos[0] != '\000') {
    esVacio = false;
  }
  return esVacio;
}

void borrarDirectorio(Disco *disco, string nombreDirectorio, int *posBloques) {
  int inodo = obtenerInodo(disco, nombreDirectorio, posBloques);
  if (disco->inodos[inodo].esDIR) {
    int *bloques = disco->inodos[inodo].posBloque;
    if (esVacio(disco, bloques)) {
      int cantBloques = cantidadBloques(posBloques);
      int i = 0;
      bool salir = false;
      while (i < cantBloques && !salir) {
        if (encontreNombre(disco->bloques[posBloques[i]].datos, nombreDirectorio)) {
          string strDatos = disco->bloques[posBloques[i]].datos;
          string strNombre = nombreDirectorio;
          strNombre += ":";
          strNombre += to_string(inodo);
          strNombre += ":";
          int pos = strDatos.find(strNombre, 0);
          strDatos.erase(pos, strNombre.length());
          strcpy(disco->bloques[posBloques[i]].datos, strDatos.c_str());
          disco->inodos[inodo].ocupado = false;
          disco->bloques[bloques[0]].ocupado = false;
          disco->inodos[inodo].posBloque[0] = -1;
          cout << "El directorio '" << nombreDirectorio << "' se borro correctamente" << endl;
          salir = true;
        } else {
          i++;
        }
      }
    } else {
      cout << "Error el directorio '" << nombreDirectorio << "' debe de ser vacio" << endl;
    }
  } else {
    cout << "Error '" << nombreDirectorio << "' no es un directorio" << endl;
  }
}

void borrarArchivo(Disco *disco, string nombreArchivo, int *posBloques) {
  int inodo = obtenerInodo(disco, nombreArchivo, posBloques);
  if (!disco->inodos[inodo].esDIR) {
    int *bloques = disco->inodos[inodo].posBloque;
    int cantBloques = cantidadBloques(posBloques);
    int i = 0;
    bool salir = false;
    while (i < cantBloques && !salir) {
      if (encontreNombre(disco->bloques[posBloques[i]].datos, nombreArchivo)) {
        string strDatos = disco->bloques[posBloques[i]].datos;
        string strNombre = nombreArchivo;
        strNombre += ":";
        strNombre += to_string(inodo);
        strNombre += ":";
        int pos = strDatos.find(strNombre, 0);
        strDatos.erase(pos, strNombre.length());
        strcpy(disco->bloques[posBloques[i]].datos, strDatos.c_str());
        disco->inodos[inodo].ocupado = false;
        for (int j = 0; j < cantidadBloques(bloques); j++) {
          disco->bloques[bloques[j]].ocupado = false;
          disco->inodos[inodo].posBloque[j] = -1;
          strcpy(disco->bloques[bloques[j]].datos, "");
        }
        cout << "El directorio '" << nombreArchivo << "' se borro correctamente" << endl;
        salir = true;
      } else {
        i++;
      }
    }
  } else {
    cout << "Error '" << nombreArchivo << "' no es un archivo" << endl;
  }
}

/*
void imprimirBloque(Disco *disco) {
  int cantBlo = cantidadBloques(disco->inodos[0].posBloque);
  for (int i = 0; i < cantBlo; i++) {
    int blocke = disco->inodos[0].posBloque[i];
    cout << disco->bloques[blocke].datos << endl;
  }
}
 */
