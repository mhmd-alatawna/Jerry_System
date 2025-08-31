#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H
#include "Defs.h"

typedef struct myKeyValuePair* KeyValuePair ;

//receives all relevant information about the two objects which will be saved in this ADT as well as the two objects themselves .
//this function creates copies of the provided two objects thus requiring destroying them later .
KeyValuePair createKeyValuePair(CopyFunction copy1 , FreeFunction free1 , PrintFunction print1 , EqualFunction equals1 ,
    CopyFunction copy2 , FreeFunction free2 , PrintFunction print2 ,Element value , Element key);

//symmetrically to the create function , destroys the two copies created in the create function as well as freeing the memory allocated for the relevant information saved
status destroyKeyValuePair(KeyValuePair pair);

//print the value using the provided printValue
status displayValue(KeyValuePair pair) ;

//prints the key using the provided function printKey
status displayKey(KeyValuePair pair) ;

//returns the value saved in the ADT , note that this time we don't return a copy , we return the value itself
Element getValue(KeyValuePair pair) ;

//returns the key saved in the ADT , note that this time we don't return a copy , we return the key itself
Element getKey(KeyValuePair pair) ;

//checks if the provided pair have an equal key as provided , this function uses the equalFunction provided in the create function
bool isEqualKey(KeyValuePair pair , Element key) ;

//creates a deep copy of the provided pair
KeyValuePair copyKeyValuePair(KeyValuePair pair) ;

#endif //KEYVALUEPAIR_H
