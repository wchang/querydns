All: main
CC = g++
CFLAGS = -O -Wall
LIBS = -lcares

main : querydns.o
	$(CC) -o qdns.exe querydns.o $(LIBS)
querydns.o : querydns.cc querydns.h 
	$(CC) $(CFLAGS) -c querydns.cc
clean:
	rm querydns.o qdns.exe
