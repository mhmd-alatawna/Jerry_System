#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Defs.h"

typedef struct list* LinkedList;

//this function receives all relevant information about the objects which will be saved in the ADT
//such as how to compare them or how to copy them , all information is provided as functions
//this function simply initialize the ADT by allocating a sizeof(list) bytes to save relevant information .
LinkedList createLinkedList(CopyFunction copyFunction ,FreeFunction freeFunction , PrintFunction printFunction , EqualFunction equalFunction , EqualFunction keyEqualFunction);

//in this function we want to destroy all information being stored at the ADT
//when creating a node we always make a copy of the provided value thus all the values in the ADT
//are allocated by us which means we are the ones who should destroy them , this is alongside the nodes themselves
//thus firstly destroy all the nodes currently in the ADT and then free the allocated memory for the list struct.
status destroyList(LinkedList linkedList);

//a function which gets a value and add it to the end of the ADT , returns alreadyExists if the value in the ADT , success if added
status appendNode(LinkedList linkedList, Element data);

//a function that receives a value and deletes it from the ADT if it exists , returns success if deleted and if not found returns failure
status deleteNode(LinkedList linkedList, Element data);

//prints the values one after the other by using the printValue function without any additions
status displayList(LinkedList linkedList);

//return the value which is currently in the 'index' place
Element getDataByIndex(LinkedList linkedList, int index);

//return the number of values inside the ADT
int getLengthList(LinkedList linkedList);

//this function uses a special provided function which is able to compare a value with a sub-value which is also
//defined by the user thus allowing the ADT to find values by knowing only part of them
Element searchByKeyInList(LinkedList linkedList, Element key);

//this function creates a deep copy of the current linked list
LinkedList copyLinkedList(LinkedList linkedList);
#endif //LINKEDLIST_H
