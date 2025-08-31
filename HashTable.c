#include "HashTable.h"

#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"
#include "KeyValuePair.h"
#include "Defs.h"

struct hashTable_s {
    int numOfElements;
    int length;
    LinkedList* list ;

    TransformIntoNumberFunction transformFunction;

    CopyFunction copyKey ;
    CopyFunction copyValue ;
    FreeFunction freeKey ;
    FreeFunction freeValue ;
    PrintFunction printKey ;
    PrintFunction printValue ;
    EqualFunction equalKey ;
};

//a function which prints a keyValuePair as a key and value
static status printPair(KeyValuePair pair) {
    if (pair == NULL) {
        return success ;
    }
    if (displayKey(pair) != success) {
        return failure ;
    }
    if (displayValue(pair) != success) {
        return failure ;
    }
    return success ;
}

//a function that compares between two keyValuePairs only by comparing their keys
static bool isEqualsPair(KeyValuePair pair1 , KeyValuePair pair2) {
    if (pair1 == NULL || pair2 == NULL) {
        return pair1 == pair2 ;
    }
    return isEqualKey(pair1 , getKey(pair2)) ;
}

//the implementation of the hashTable will be as a linkedList of keyValuePairs , for this
//purpose we will initialize a constant number of empty LinkedLists and provide them
//information to how to compare,copy and other functionalities about keyValuePairs
hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey,
    PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue,
    PrintFunction printValue, EqualFunction equalKey,
    TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    
    if(copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL)
        return NULL ;
    if(transformIntoNumber == NULL || hashNumber <= 0)
        return NULL ;
    
    hashTable table = (hashTable) malloc(sizeof(struct hashTable_s));
    if (table == NULL) {
        return NULL ;
    }
    table->numOfElements = 0;
    table->length = hashNumber;
    table->transformFunction = transformIntoNumber;

    table->copyKey = copyKey;
    table->copyValue = copyValue;
    table->freeKey = freeKey;
    table->freeValue = freeValue;
    table->printKey = printKey;
    table->printValue = printValue;
    table->equalKey = equalKey;

    table->list = (LinkedList*) malloc(sizeof(LinkedList)*hashNumber);
    if (table->list == NULL) {
        return NULL ;
    }
    for (int i = 0; i < hashNumber; i++) {
        table->list[i] = createLinkedList(copyKeyValuePair , destroyKeyValuePair ,printPair , isEqualsPair , isEqualKey);
    }
    return table ;
}

//destroys all the linked lists in the ADT thus freeing all saved memory , then free the struct itself which
//contains the pointers to the functions provided in the create function
status destroyHashTable(hashTable table){
    if (table == NULL) {
        return success ;
    }
    for (int i = 0 ; i < table->length; i++) {
        if (destroyList(table->list[i]) != success) {
            return failure ;
        }
    }
    free(table->list);
    free(table);
    return success ;
}

//adds a new key-value pair to the ADT , this will allow searching by key later on
status addToHashTable(hashTable table, Element key,Element value) {
    if (key == NULL || value == NULL || table == NULL) {
        return argumentsProblem ;
    }

    int index = table->transformFunction(key) % table->length ; //apply hash function on the key to find the corresponding index to add the value to .
    LinkedList listToAddTo = table->list[index]; //this is the list we want to add the value to .
    KeyValuePair newPair = createKeyValuePair(table->copyKey , table->freeKey , table->printKey , table->equalKey,
        table->copyValue , table->freeValue , table->printValue, value , key) ;//create a keyValuePair using the provided information
    status result = appendNode(listToAddTo, newPair) ; //add the keyValuePair to the wanted linkedList
    destroyKeyValuePair(newPair) ; //destroy the created keyValuePair due to the fact that LinkedList appendNode function creates a copy of the saved element
    table->numOfElements ++ ; //update number of elements in the table
    return result ;
}

//search a given key in the ADT , return the corresponding added value if found otherwise return NULL
Element lookupInHashTable(hashTable table, Element key) {
    if (table == NULL || key == NULL) {
        return NULL ;
    }
    int index = table->transformFunction(key) % table->length ;//apply hash function to the key
    LinkedList listToLookup = table->list[index]; //find the corresponding linkedList
    KeyValuePair result = searchByKeyInList(listToLookup , key) ; //search by only the key in the found linkedList
    if (result == NULL) {
        return NULL ;
    }
    return getValue(result) ; //return only the value
}

//remove a key-value pair from the ADT
status removeFromHashTable(hashTable table, Element key) {
    if (table == NULL || key == NULL) {
        return failure ;
    }
    int index = table->transformFunction(key) % table->length ;//apply hash function on key
    LinkedList listToLookup = table->list[index];//find corresponding linkedlist
    KeyValuePair result = searchByKeyInList(listToLookup , key) ; //look ofr the key , if not found exit
    if (result == NULL) {
        return failure ;
    }
    if (deleteNode(listToLookup , result) != success)//if found then remove it from the linkedList
        return failure;
    table->numOfElements -- ;
    return success ;
}

//prints all the elements saved in the ADT as well as their corresponding keys
status displayHashElements(hashTable table) {
    if (table == NULL) {
        return success ;
    }
    for (int i = 0 ; i < table->length; i++) {//for each list in the table display it .
        if (displayList(table->list[i]) != success) {
            return failure ;
        }
    }
    return success ;
}