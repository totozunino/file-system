all: disk makedir fs

disk: crear_disco.cpp file_system.o
	g++ -Wall -g crear_disco.cpp file_system.o -o crear_disco

makedir: crear_dir.cpp file_system.o
	g++ -Wall -g crear_dir.cpp file_system.o -o crear_dir

fs: file_system.cpp file_system.hh
	g++ -Wall -g -c file_system.cpp 

clean:
	rm -f *.o
