CC = gcc
CFLAGS = -pedantic -ansi -Werror
OBJ = main.o linkedList.o readFile.o writeFile.o currentTime.o
EXEC = cq

cq : main.o readFile.o writeFile.o linkedList.o currentTime.o
	gcc main.o readFile.o writeFile.o linkedList.o currentTime.o -o cq

main.o: main.c readFile.c readFile.h writeFile.c currentTime.c writeFile.h linkedList.c linkedList.h currentTime.h
	$(CC) -c main.c $(CFLAGS) readFile.c $(CFLAGS) writeFile.c $(CFLAGS) linkedList.c $(CFLAGS) currentTime.c $(CFLAGS)

readFile.o: readFile.c readFile.h linkedList.c linkedList.h
	$(CC) -c readFile.c $(CFLAGS) linkedList.c $(CFLAGS)
	
writeFile.o: writeFile.c writeFile.h
	$(CC) -c writeFile.c $(CFLAGS)

linkedList.o: linkedList.c linkedList.h
	$(CC) -c linkedList.c $(CFLAGS)
	
currentTime.o: currentTime.c currentTime.h
	$(CC) -c currentTime.c $(CFLAGS)

clean:
		rm -f $(EXEC) $(OBJ)
