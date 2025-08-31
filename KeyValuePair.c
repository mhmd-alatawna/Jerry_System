#include "KeyValuePair.h"

#include <stdlib.h>

struct myKeyValuePair {
    Element key ;
    Element value ;
    CopyFunction keyCopy ;
    CopyFunction valueCopy ;
    FreeFunction keyFree ;
    FreeFunction valueFree ;
    PrintFunction keyPrint ;
    PrintFunction valuePrint ;
    EqualFunction keyEqual ;
};


KeyValuePair createKeyValuePair(CopyFunction copy1 , FreeFunction free1 , PrintFunction print1 , EqualFunction equals1 ,
    CopyFunction copy2 , FreeFunction free2 , PrintFunction print2,
    Element value , Element key) {
    if (key == NULL || value == NULL) {
        return NULL ;
    }
    if(copy1 == NULL || copy2 == NULL || free1 == NULL || free2 == NULL || print1 == NULL || print2 == NULL || equals1 == NULL)
        return NULL ;
    KeyValuePair pair = (KeyValuePair) malloc(sizeof(struct myKeyValuePair));
    if (pair == NULL) {
        return NULL ;
    }
    pair->keyCopy = copy1 ;
    pair->valueCopy = copy2 ;
    pair->keyFree = free1 ;
    pair->valueFree = free2 ;
    pair->keyPrint = print1 ;
    pair->valuePrint = print2 ;
    pair->keyEqual = equals1 ;

    pair->key = pair->keyCopy(key) ;//create a copy of the key
    if (pair->key == NULL) {
        return NULL ;
    }
    pair->value = pair->valueCopy(value) ; //create a copy of the value
    if (pair->value == NULL) {
        pair->keyFree(pair->key) ;
        return NULL ;
    }
    return pair ;
}

status destroyKeyValuePair(KeyValuePair pair) {
    if (pair == NULL) {
        return success ;
    }
    if (pair->valueFree(pair->value) != success) {//free value copy
        return failure ;
    }
    if (pair->keyFree(pair->key) != success) {//free key copy
        return failure ;
    }
    free(pair) ; //free the allocated memory from create function
    return success ;
}

status displayValue(KeyValuePair pair){
    if (pair == NULL) {
        return failure ;
    }
    return pair->valuePrint(pair->value) ;
}

status displayKey(KeyValuePair pair){
    if (pair == NULL) {
        return failure ;
    }
    return pair->keyPrint(pair->key) ;
}

Element getKey(KeyValuePair pair) {
    if (pair == NULL) {
        return NULL ;
    }
    return pair->key ;
}

Element getValue(KeyValuePair pair) {
    if (pair == NULL) {
        return NULL ;
    }
    return pair->value ;
}

bool isEqualKey(KeyValuePair pair , Element key) {
    if (pair == NULL) {
        return false ;
    }
    return pair->keyEqual(pair->key, key) ;
}

//note that the key and value passed here are simply the ones saved in pair , this is because the copy is being
//created in the create function thus no need to create another copy here .
KeyValuePair copyKeyValuePair(KeyValuePair pair) {
    if (pair == NULL) {
        return NULL ;
    }
    KeyValuePair newPair = createKeyValuePair(pair->keyCopy , pair->keyFree ,
        pair->keyPrint ,pair->keyEqual , pair->valueCopy , pair->valueFree ,
        pair->valuePrint ,pair->value , pair->key) ;
    return newPair ;
}