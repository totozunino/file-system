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
      if (clave != -1) {
        // Creo la memoria compartida
        int mem_id = shmget(clave, sizeof(_strDisco), IPC_CREAT | 0666);
        if (mem_id != -1) {
          // Obtengo la memoria compartida
          Disco *disco = (Disco *) shmat(mem_id, NULL, 0);
          if (disco != (void *) -1) {
            disco->inodos[0].esDIR = true;
            disco->inodos[0].ocupado = true;
            disco->inodos[0].posBloque[0] = 0;
            disco->bloques[0].ocupado = true;
            // Libero la memoria compartida
            if (shmdt(disco) != -1) {
              cout << "Disco creado correctamente" << endl;
            } else {
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