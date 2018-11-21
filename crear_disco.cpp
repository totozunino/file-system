#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 2) {
    key_t clave = ftok(argv[0], 25);
    int mem_id = shmget(clave, 1024, 0777 | IPC_CREAT);
    int *a;
    a = (int *) shmat(mem_id, NULL, NULL);
    cout << "Se creo la memoria compartida correctamente" << endl;
  } else {
    cout << "Error, es necesario ingresar la direccion del disco" << endl;
  }

  return 0;
}