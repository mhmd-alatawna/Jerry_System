#ifndef MULTIVALUEHASHTABLE_H
#define MULTIVALUEHASHTABLE_H
#include "Defs.h"
#include "LinkedList.h"

typedef struct multi_table* MultiHashTable;

//initialize an empty ADT which takes in the relevant information about the objects which will be saved in the ADT
MultiHashTable createMultiHashTable(CopyFunction copyKey , FreeFunction freeKey , PrintFunction printKey ,
    EqualFunction equalKey,CopyFunction copyValue , FreeFunction freeValue ,
    PrintFunction printValue , EqualFunction equalValue , TransformIntoNumberFunction transformFunction , int length);

//destroy all the data added to the ADT or created by the create function
status destroyMultiHashTable(MultiHashTable multiHashTable) ;
//add a new key-pair pair to the ADT , returns success if successfully added
status addToMultiHashTable(MultiHashTable multiHashTable, Element key, Element value) ;
//return a corresponding LinkedList of values to the given key which is saved in the ADT , if not found
//then return NULL
LinkedList lookUpInMultiHashTable(MultiHashTable multiHashTable, Element key) ;
//removes one key-value pair from the ADT , and updates the ADT accordingly
status removeFromMultiHashTable(MultiHashTable multiHashTable, Element key , Element value) ;
//display all elements in the ADT in a specific format
status displayMultiValueHashElementsByKey(MultiHashTable multiHashTable, Element key) ;
#endif //MULTIVALUEHASHTABLE_H
