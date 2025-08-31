#include "MultiValueHashTable.h"
#include "HashTable.h"
#include "LinkedList.h"
#include <stdlib.h>

struct multi_table {
    hashTable table;
    CopyFunction copyValue ;
    FreeFunction freeValue ;
    PrintFunction printValue ;
    EqualFunction equalValue ;

    PrintFunction printKey ;
};

//this ADT will be implemented as a hashTable of LinkedLists thus initialize it accordingly , firstly initialize
//the hashTable and then allocate memory to save provided functionality .
MultiHashTable createMultiHashTable(CopyFunction copyKey , FreeFunction freeKey , PrintFunction printKey ,
        EqualFunction equalKey,CopyFunction copyValue , FreeFunction freeValue ,PrintFunction printValue ,
        EqualFunction equalValue , TransformIntoNumberFunction transformFunction , int length) {

    if(copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL || printValue == NULL || equalKey == NULL || equalValue == NULL)
        return NULL ;
    if(transformFunction == NULL || length <= 0)
        return NULL ;
        
    MultiHashTable multiHashTable = malloc(sizeof(struct multi_table));
    if (multiHashTable == NULL) {
        return NULL;
    }
    multiHashTable->table = createHashTable(copyKey , freeKey , printKey , copyLinkedList , destroyList , displayList , equalKey , transformFunction , length) ;
    if (multiHashTable->table == NULL) {
        return NULL ;
    }
    multiHashTable->copyValue = copyValue ;
    multiHashTable->freeValue = freeValue ;
    multiHashTable->printValue = printValue ;
    multiHashTable->equalValue = equalValue ;
    multiHashTable->printKey = printKey ;
    return multiHashTable ;
}

//destroy the hashTable and then free the allocated memory from create function
status destroyMultiHashTable(MultiHashTable multiHashTable) {
    if (multiHashTable == NULL) {
        return success ;
    }
    status result = destroyHashTable(multiHashTable->table);
    free(multiHashTable) ;
    return result ;
}

//look for the provided key in the hashTable , if found then simply add the value
//to the corresponding LinkedList which will be returned by the lookUp function .
//otherwise add an empty LinkedList to the hashTable corresponding to the given key
//and after that add the given value to the newly added LinkedList
status addToMultiHashTable(MultiHashTable multiHashTable, Element key, Element value) {
    status result ;
    if (multiHashTable == NULL || key == NULL || value == NULL) {
        return argumentsProblem ;
    }
    LinkedList listToAddTo = lookupInHashTable(multiHashTable->table , key) ;
    if (listToAddTo == NULL) {
        listToAddTo = createLinkedList(multiHashTable->copyValue , multiHashTable->freeValue , multiHashTable->printValue , multiHashTable->equalValue, NULL) ;
        if (listToAddTo == NULL) {
            return failure ;
        }
        result = addToHashTable(multiHashTable->table , key , listToAddTo) ;
        if (result != success) {
            return result ;
        }
        destroyList(listToAddTo) ;
    }

    listToAddTo = lookupInHashTable(multiHashTable->table , key) ;
    result = appendNode(listToAddTo , value) ;
    if (result != success) {
        return result ;
    }
    return success ;
}

//simply lookUp in the hashtable and return its result
LinkedList lookUpInMultiHashTable(MultiHashTable multiHashTable, Element key) {
    if (multiHashTable == NULL || key == NULL) {
        return NULL ;
    }
        return (LinkedList) lookupInHashTable(multiHashTable->table , key) ;
}


//lookUp the key , if not found return failure , if found then attempt to remove
//the value from the corresponding linkedlist , if failed then return with failure , otherwise
//the value will be deleted , now if the list is empty remove the key and the corresponding empty list from the hash table
status removeFromMultiHashTable(MultiHashTable multiHashTable, Element key , Element value) {
    if (multiHashTable == NULL || key == NULL) {
        return failure ;
    }
    LinkedList listToLookUp = lookupInHashTable(multiHashTable->table , key) ;
    if (listToLookUp == NULL) {
        return failure ;
    }
    if (deleteNode(listToLookUp , value) != success) {
        return failure ;
    }

    if (getLengthList(listToLookUp) == 0) {
        if (removeFromHashTable(multiHashTable->table , key) != success) {
            return failure ;
        }
    }
    return success ;
}


status displayMultiValueHashElementsByKey(MultiHashTable multiHashTable, Element key) {
    if (multiHashTable == NULL || key == NULL) {
        return argumentsProblem ;
    }
    LinkedList listToDisplay = lookupInHashTable(multiHashTable->table , key) ;
    if (listToDisplay == NULL) {
        return failure ;
    }
    if (multiHashTable->printKey(key) != success) {
        return failure ;
    }
    if (displayList(listToDisplay) != success) {
        return failure ;
    }
    return success ;
}