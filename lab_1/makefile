OBJURL = build/
SOURCEURL = source/
CC=gcc

CFLAGS=-c -Wall -g

dummy_build_folder := $(shell mkdir -p $(OBJURL)) #Создание папки

all: lab_1

lab_1: lab_1.o
	$(CC) $(OBJURL)lab_1.o -o lab_1 -lm

lab_1.o: $(SOURCEURL)lab_1.c
	$(CC) $(CFLAGS) $(SOURCEURL)lab_1.c -o $(OBJURL)lab_1.o

clean:
	rm -rf $(OBJURL)*.o lab_1