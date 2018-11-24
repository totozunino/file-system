all: disk 

disk: crear_disco.cpp 
	g++ -Wall -g crear_disco.cpp -o crear_disco

clean:
	rm -f *.o
