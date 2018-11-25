all: disk rmdisk makedir showdir fs

disk: crear_disco.cpp file_system.o
	g++ -Wall -g crear_disco.cpp file_system.o -o crear_disco

rmdisk: borrar_disco.cpp file_system.o
	g++ -Wall -g borrar_disco.cpp file_system.o -o borrar_disco

makedir: crear_dir.cpp file_system.o
	g++ -Wall -g crear_dir.cpp file_system.o -o crear_dir

showdir: muestra_dir.cpp file_system.o
	g++ -Wall -g muestra_dir.cpp file_system.o -o muestra_dir

fs: file_system.cpp file_system.hh
	g++ -Wall -g -c file_system.cpp 

clean:
	rm -f *.o
