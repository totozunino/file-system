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

int main(int argc, char *argv[]) {
  if (argc == 2) {
    int largo = largoRuta(argv[1]);
    string strArray[largo];
    parsearRuta(strArray, largo, argv[1]);
    string s = "/tmp/";
    s += strArray[0];
    key_t clave = ftok(s.c_str(), 25);
    if (clave != -1) {
      int mem_id = shmget(clave, sizeof(_strDisco), 0);
      if (mem_id != -1) {
        Disco *disco = (Disco *) shmat(mem_id, NULL, 0);
        if (disco != (void *) -1) {
          int inodo;
          if (existeRuta(disco, strArray, largo, inodo)) {
            if (largo > 1) {
              int *posBloques = disco->inodos[inodo].posBloque;
              if (existeDirectorio(disco, strArray[largo - 1], posBloques)) {
                imprimirArchivo(disco, strArray[largo - 1], posBloques);
                if (shmdt(disco) == -1) {
                  cout << "Error al liberar la memoria compartida" << endl;
                }
              } else {
                cout << "Error el archivo '" << strArray[largo - 1] << "' no existe" << endl;
              }
            } else {
              cout << "Error '" << argv[1] << "' no es un archivo" << endl;
            }
          } else {
            cout << "Error la ruta igresada no es valida" << endl;
          }
        } else {
          cout << "Error al obtener la memoria compartida" << endl;
        }
      } else {
        cout << "Error el disco '" << strArray[0] << "' no existe" << endl;
      }
    } else {
      cout << "Error el disco '" << strArray[0] << "' no existe" << endl;
    }
  } else {
    cout << "Error ingrese correctamente el archivo que desea leer" << endl;
  }
}
