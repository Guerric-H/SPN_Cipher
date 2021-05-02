CC=gcc
CFLAGS=-Wall -O3 -I headers/present24/
SRC=src/present24/
RELEASE=release/src/present24/
HEADERS=headers/present24/
COMPILE=release/present24

all: $(COMPILE)

run: 
	./$(COMPILE)

clean:
	rm $(RELEASE)*.o
	rm release/src/main.o
	rm release/present24


$(COMPILE): release/src/main.o $(RELEASE)Encryption.o $(RELEASE)Decryption.o $(RELEASE)Attack.o $(RELEASE)KeyGeneration.o $(RELEASE)Interface.o 
	$(CC) -o $(COMPILE) $^ $(CFLAGS)

release/src/main.o: src/main.c $(HEADERS)*.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(RELEASE)Encryption.o: $(SRC)Encryption.c $(HEADERS)Encryption.h $(HEADERS)KeyGeneration.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(RELEASE)Decryption.o: $(SRC)Decryption.c  $(HEADERS)Decryption.h $(HEADERS)KeyGeneration.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(RELEASE)Attack.o: $(SRC)Attack.c  $(HEADERS)Attack.h $(HEADERS)KeyGeneration.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(RELEASE)KeyGeneration.o: $(SRC)KeyGeneration.c $(HEADERS)KeyGeneration.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(RELEASE)Interface.o :$(SRC)Interface.c $(HEADERS)Interface.h
	$(CC) -o $@ -c $< $(CFLAGS)
