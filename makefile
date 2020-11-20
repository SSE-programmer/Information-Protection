OBJURL = build/
SOURCEURL = source/
CC=gcc

CFLAGS=-c -Wall -g

dummy_build_folder := $(shell mkdir -p $(OBJURL)) #Создание папки

all: main
   # - ./lab_1

main: main.o cryptbaselib.o encryptionlib.o digital_signature.o mental_poker.o blind_signature.o
	$(CC) $(OBJURL)cryptbaselib.o $(OBJURL)encryptionlib.o $(OBJURL)digital_signature.o $(OBJURL)mental_poker.o $(OBJURL)blind_signature.o $(OBJURL)main.o -o main -lm -lcrypto

main.o: $(SOURCEURL)main.c
	$(CC) $(CFLAGS) $(SOURCEURL)main.c -o $(OBJURL)main.o

cryptbaselib.o: $(SOURCEURL)cryptbaselib.c
	$(CC) $(CFLAGS) $(SOURCEURL)cryptbaselib.c -o $(OBJURL)cryptbaselib.o

encryptionlib.o: $(SOURCEURL)encryptionlib.c
	$(CC) $(CFLAGS) $(SOURCEURL)encryptionlib.c -o $(OBJURL)encryptionlib.o

digital_signature.o: $(SOURCEURL)digital_signature.c
	$(CC) $(CFLAGS) $(SOURCEURL)digital_signature.c -o $(OBJURL)digital_signature.o

mental_poker.o: $(SOURCEURL)mental_poker.c
	$(CC) $(CFLAGS) $(SOURCEURL)mental_poker.c -o $(OBJURL)mental_poker.o

blind_signature.o: $(SOURCEURL)blind_signature.c
	$(CC) $(CFLAGS) $(SOURCEURL)blind_signature.c -o $(OBJURL)blind_signature.o

clean:
	rm -rf $(OBJURL)*.o lab_1
