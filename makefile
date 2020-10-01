OBJURL = build/
SOURCEURL = source/
CC=gcc

CFLAGS=-c -Wall -g

dummy_build_folder := $(shell mkdir -p $(OBJURL)) #Создание папки

all: main
   # - ./lab_1

main: main.o cryptlib.o
	$(CC) $(OBJURL)cryptlib.o $(OBJURL)main.o -o main -lm

main.o: $(SOURCEURL)main.c
	$(CC) $(CFLAGS) $(SOURCEURL)main.c -o $(OBJURL)main.o

cryptlib.o: $(SOURCEURL)cryptlib.c
	$(CC) $(CFLAGS) $(SOURCEURL)cryptlib.c -o $(OBJURL)cryptlib.o

clean:
	rm -rf $(OBJURL)*.o lab_1