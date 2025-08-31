#include "LinkedList.h"

#include <stdio.h>
#include <stdlib.h>

//this struct will contain a single value in this ADT
typedef struct Node1 Node ;
struct Node1 {
    Element value ;
    Node* next ;
};

//the classic implementation of linked list with a head and tail to make appending faster
struct list {
    Node* head ;
    Node* tail ;
    int length ;
    CopyFunction copyFunction ;
    FreeFunction freeFunction ;
    PrintFunction printFunction ;
    EqualFunction equalFunction ;
    EqualFunction keyEqualFunction ;
};

//in this function a node is being created , as well as a copy of the value provided by the user
static Node* createNode (LinkedList linkedList , Element value ) {
    Node* newNode = (Node*) malloc(sizeof(Node)) ;
    if (newNode == NULL) {
        return NULL ;
    }
    newNode->value = linkedList->copyFunction(value) ;
    newNode->next = NULL ;
    if (newNode->value == NULL) {
        free(newNode) ;
        return NULL ;
    }
    return newNode ;
}

//the symmetric function to createNode which will destroy any work done in the create function
//thus it firstly destroys the copy of the value which is being created in the create function and then destroy the node it self
static status destroyNode(LinkedList linkedList , Node* node) {
    if (node == NULL) {
        return success ;
    }
    if (linkedList->freeFunction(node->value) != success) {
        free(node) ;
        return failure ;
    };
    free(node) ;
    return success ;
}

LinkedList createLinkedList(CopyFunction copyFunction ,FreeFunction freeFunction , PrintFunction printFunction , EqualFunction equalFunction, EqualFunction keyEqualFunction) {
    if(copyFunction == NULL || freeFunction == NULL || printFunction == NULL || equalFunction == NULL)
        return NULL ;
    LinkedList linkedList = (LinkedList) malloc(sizeof(struct list)) ;
    if (linkedList == NULL) {
        return NULL ;
    }
    linkedList->head = NULL ;
    linkedList->tail = NULL ;
    linkedList->length = 0 ;
    linkedList->copyFunction = copyFunction ;
    linkedList->freeFunction = freeFunction ;
    linkedList->printFunction = printFunction ;
    linkedList->equalFunction = equalFunction ;
    linkedList->keyEqualFunction = keyEqualFunction ;
    return linkedList ;
}

status destroyList(LinkedList linkedList) {
    if (linkedList == NULL) {
        return argumentsProblem ;
    }
    if (linkedList->head == NULL) {//base case
        free(linkedList) ;
        return success ;
    }
    Node* curr = linkedList->head ; //will have the current node that should be destroyed
    Node* tmp = NULL ;//used to store the pointer to the next node which will be lost if not saved due to the destruction of the current node
    status result = success ;
    while (curr != NULL) {//in each iteration destroy the current node and go on to the next one
        tmp = curr->next ;
        if (destroyNode(linkedList , curr) != success) {
            result = failure ;
        }
        curr = tmp ;
    }
    free(linkedList) ; //free the list struct after being done with the rest
    return result ;
}

//a function which will return true if the value provided already exists in the ADT otherwise false
static bool isFound (LinkedList linkedList, Element value) {
    if (linkedList == NULL) {
        return argumentsProblem ;
    }
    Node* curr = linkedList->head ;
    while (curr != NULL) {//check in each iteration if the current node holds the provided value (or a copy of it)
        if (linkedList->equalFunction(value, curr->value)) {
            return true ;
        }
        curr = curr->next ;
    }
    return false ;
}

//a function which gets a value and add it to the end of the ADT
status appendNode(LinkedList linkedList, Element data) {
    if (linkedList == NULL || data == NULL) {
        return argumentsProblem ;
    }
    if (isFound(linkedList , data)) {
        return alreadyExists ;
    }

    Node* newNode = createNode(linkedList, data) ; //wrap the value with a node (note that from now on we are working with a copy)
    if (newNode == NULL) {
        return memoryProblem ;
    }
    if (linkedList->length == 0) {//if there are no values in the ADT just update the corresponding fields accordingly
        linkedList->head = newNode ;
        linkedList->tail = newNode ;
        linkedList->length ++ ;
        return success ;
    }
    linkedList->length ++ ; //add to the end of the list and then update tail
    linkedList->tail->next = newNode ;
    linkedList->tail = newNode ;
    return success ;
}

//a function that receives a value and deletes it from the ADT if it exists .
status deleteNode(LinkedList linkedList, Element data) {
    if (linkedList == NULL || data == NULL) {
        return argumentsProblem ;
    }
    Node* curr = linkedList->head ;
    Node* prev = NULL ;
    bool found = false ;
    while (curr != NULL) {
        if (linkedList->equalFunction(curr->value,data) == true) {//check if current holds the correct value
            //in case the current is the head or tail then update head and tail accordingly
            found = true ;
            if (curr == linkedList->head) {
                linkedList->head = curr->next ;
            }else if (curr == linkedList->tail) {
                linkedList->tail = prev ;
                prev->next = NULL ;
            }else {
                prev->next = curr->next ;
            }
            linkedList->length -- ;
            destroyNode(linkedList, curr) ;
            if (linkedList->length == 0) {//in case there are no elements in the ADT update fields accordingly
                linkedList->tail = NULL ;
                linkedList->head = NULL ;
            }
        }
        if (found == true) //if found just break no need to continue
            break;
        prev = curr ;
        curr = curr->next ;
    }
    if (found == true)
        return success ;
    return failure ; //if not found
}

status displayList(LinkedList linkedList) {
    if (linkedList == NULL) {
        return success ;
    }
    Node* curr = linkedList->head ;
    while (curr != NULL) {//simply go through the list and print the current value
        if (linkedList->printFunction(curr->value) != success) {
            return failure ;
        };
        curr = curr->next ;
    }
    return success ;
}

//this function will return the data saved in a corresponding index , it will simply do 'index' skips from the head till it reaches the needed value
Element getDataByIndex(LinkedList linkedList, int index) {
    if (linkedList == NULL || index <= 0 || index > linkedList->length) {
        return NULL ;
    }
    Node* curr = linkedList->head ;
    while (index > 1) {
        curr = curr->next ;
        index --;
    }
    return curr->value ;
}

int getLengthList(LinkedList linkedList) {
    if (linkedList == NULL) {
        return -1 ;
    }
    return linkedList->length ;
}

//this function uses a special provided function which is able to compare a value with a sub-value which is also
//defined by the user thus allowing the ADT to find values by knowing only part of them
Element searchByKeyInList(LinkedList linkedList, Element key) {
    if (linkedList == NULL || key == NULL) {
        return NULL ;
    }
    Node* curr = linkedList->head ;
    while (curr != NULL) {//in each iteration check if the current value contains the sub-value if so return the value
        if (linkedList->keyEqualFunction(curr->value,key) == true) {
            return curr->value ;
        }
        curr = curr->next ;
    }
    return NULL ;
}


//this function creates a deep copy of the current linked list
LinkedList copyLinkedList(LinkedList linkedList) {
    LinkedList newList = createLinkedList(linkedList->copyFunction , linkedList->freeFunction , linkedList->printFunction , linkedList->equalFunction , linkedList->keyEqualFunction) ;
    if (newList == NULL) {
        return NULL ;
    }
    Node* curr = linkedList->head ;
    while (curr != NULL) {
        appendNode(newList, curr->value) ;
        curr = curr->next ;
    }
    return newList ;
}