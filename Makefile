All: main
CC = g++
CFLAGS = -O -Wall
LIBS = -lcares
OBJS = querydns.o resolveparameters.o

main : $(OBJS)
	$(CC) -g -o qdns.exe $(OBJS) $(LIBS)
querydns.o : querydns.cc querydns.h 
	$(CC) $(CFLAGS) -c querydns.cc
resolveparameters.o : resolveparameters.cc
	$(CC) $(CFLAGS) -c resolveparameters.cc
clean:
	rm *.o qdns.exe

