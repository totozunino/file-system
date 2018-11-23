all: disk fs

disk: crear_disco.cpp file_system.o
	g++ -Wall crear_disco.cpp file_system.o -o crear_disco

fs: file_system.cpp file_system.hh
	g++ -Wall -c file_system.cpp

clean:
	rm -f *.o
