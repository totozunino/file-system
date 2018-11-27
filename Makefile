all: disk rmdisk makedir showdir makefile showfile fs

disk: crear_disco.cpp file_system.o
	g++ -Wall -g crear_disco.cpp file_system.o -o crear_disco

rmdisk: borrar_disco.cpp file_system.o
	g++ -Wall -g borrar_disco.cpp file_system.o -o borrar_disco

makedir: crear_dir.cpp file_system.o
	g++ -Wall -g crear_dir.cpp file_system.o -o crear_dir

showdir: muestra_dir.cpp file_system.o
	g++ -Wall -g muestra_dir.cpp file_system.o -o muestra_dir

makefile: crear_archivo.cpp file_system.o
	g++ -Wall -g crear_archivo.cpp file_system.o -o creo_archivo
	
showfile: leo_archivo.cpp file_system.o
	g++ -Wall -g leo_archivo.cpp file_system.o -o leo_archivo

fs: file_system.cpp file_system.hh
	g++ -Wall -g -c file_system.cpp 

clean:
	rm -f *.o
