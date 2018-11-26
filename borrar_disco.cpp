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
    string s = "/tmp/";
    s += argv[1];
    int archivo = open(s.c_str(), O_RDONLY | O_CREAT | O_EXCL, 0666);
    if (archivo == -1) {
      key_t clave = ftok(s.c_str(), 25);
      if (clave != -1) {
        int mem_id = shmget(clave, sizeof(_strDisco), 0);
        if (mem_id != -1) {
          if (shmctl(mem_id, IPC_RMID, NULL) != -1) {
            remove(s.c_str());
            cout << "El disco " << argv[1] << " se borro correctamente" << endl;
          } else {
            cout << "Error al borrar el disco" << endl;
          }
        } else {
          cout << "Error al obtener la memoria compartida" << endl;
        }
      } else {
        remove(s.c_str());
        cout << "Error el disco '" << argv[1] << "' no existe" << endl;
      }
    } else {
      remove(s.c_str());
      cout << "Error el disco '" << argv[1] << "' no existe" << endl;
    }
  } else {
    cout << "Error ingrese el nombre del disco que desea borrar correctamente" << endl;
  }
}
