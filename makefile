JerryBoree : MultiValueHashTable.o HashTable.o KeyValuePair.o LinkedList.o Jerry.o JerryBoreeMain.o
	gcc MultiValueHashTable.c HashTable.o KeyValuePair.o LinkedList.o Jerry.o JerryBoreeMain.o -o JerryBoree

Jerry.o : Jerry.c Jerry.h
	gcc -c Jerry.c

LinkedList.o : LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c

KeyValuePair.o : KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c
 
HashTable.o : HashTable.c HashTable.h Defs.h
	gcc -c HashTable.c

MultiValueHashTable.o : MultiValueHashTable.c MultiValueHashTable.h Defs.h
	gcc -c MultiValueHashTable.c

JerryBoreeMain.o : JerryBoreeMain.c Defs.h
	gcc -c JerryBoreeMain.c
 
clean :
	rm -f *.o