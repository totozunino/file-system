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
    int largo = largoDireccion(argv[1]);
    string strArray[largo];
    parsearDireccion(strArray, largo, argv[1]);
    string s = "./";
    s += strArray[0];
    key_t clave = ftok(s.c_str(), 25);
    if (clave != -1) {
      int mem_id = shmget(clave, sizeof(_strDisco), 0);
      if (mem_id != -1) {
        Disco *disco = (Disco *) shmat(mem_id, NULL, 0);
        if (disco != (void *) -1) {
          checkDirectorio(disco, strArray, largo);
        } else {
          cout << "Error al apegarse a la memoria compartida" << endl;
        }
      } else {
        cout << "Error no existe el disco" << endl;
      }
    } else {
      cout << "Error al crear la clave" << endl;
    }
  } else {
    cout << "Error, ingrese correctamente el directorio que desea crear" << endl;
  }
}
