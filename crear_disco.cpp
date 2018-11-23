#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "file_system.hh"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 2) {
    string s = "./";
    s += argv[1];
    // Creo el archivo con el nombre del disco
    int archivo = open(s.c_str(), O_RDONLY | O_CREAT | O_EXCL, 0666);
    if (archivo != -1) {
      // Creo la clave segun el archivo creado
      key_t clave = ftok(s.c_str(), 25);
      if (clave > 0) {
        // Creo la memoria compartida
        int mem_id = shmget(clave, sizeof(_strDisco), IPC_CREAT | 0666);
        if (mem_id != -1) {
          // Obtengo la memoria compartida
          Disco mem = (Disco) shmat(mem_id, NULL, 0);
          if (mem != (void *) -1) {
            crearDisco(mem);
            // Libero la memoria compartida
            if (shmdt(mem) == -1) {
              remove(s.c_str());
              cout << "Error al liberar la memoria" << endl;
            }
          } else {
            remove(s.c_str());
            cout << "Error al obtener la memoria" << endl;
          }
        } else {
          remove(s.c_str());
          cout << "Error al crear la memoria compartida" << endl;
        }
      } else {
        remove(s.c_str());
        cout << "Error al crear la clave" << endl;
      }
    } else {
      cout << "Error, el disco " << argv[1] << " ya existe " << endl;
    }
  } else {
    cout << "Error, ingrese correctamente el disco que desea crear. [./crear_disco nombre]" << endl;
  }
}