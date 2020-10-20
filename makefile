OBJURL = build/
SOURCEURL = source/
CC=gcc

CFLAGS=-c -Wall -g

dummy_build_folder := $(shell mkdir -p $(OBJURL)) #Создание папки

all: main
   # - ./lab_1

main: main.o cryptbaselib.o encryptionlib.o
	$(CC) $(OBJURL)cryptbaselib.o $(OBJURL)encryptionlib.o $(OBJURL)main.o -o main -lm

main.o: $(SOURCEURL)main.c
	$(CC) $(CFLAGS) $(SOURCEURL)main.c -o $(OBJURL)main.o

cryptbaselib.o: $(SOURCEURL)cryptbaselib.c
	$(CC) $(CFLAGS) $(SOURCEURL)cryptbaselib.c -o $(OBJURL)cryptbaselib.o

encryptionlib.o: $(SOURCEURL)encryptionlib.c
	$(CC) $(CFLAGS) $(SOURCEURL)encryptionlib.c -o $(OBJURL)encryptionlib.o

clean:
	rm -rf $(OBJURL)*.o lab_1
