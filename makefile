executableName	=	sim

all:	sure

sure:	main.o	process.o  disk.o  memory.o
	g++	-o	$(executableName)	main.o	process.o  disk.o  memory.o

main.o: main.cpp
	g++	-c	-std=c++11 main.cpp

process.o:  process.cpp
	g++	-c	-std=c++11 process.cpp

disk.o:  disk.cpp
	g++	-c	-std=c++11 disk.cpp

memory.o:  memory.cpp
	g++	-c	-std=c++11 memory.cpp

clean:	$(RM)	*.o	*~	$(executableName)
