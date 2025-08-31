#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Jerry.h"
#include "Defs.h"

status createJerry(char* ID1 , int happiness1 , Origin* originP , Jerry** jerryPP) {
    if (jerryPP == NULL ||ID1 == NULL) //check if arguments are correct
        return failure ;
    Jerry* j = (Jerry*) malloc(sizeof(Jerry)) ;
    if (j == NULL) //check if memory have been allocated correctly
        return memoryProblem ;
    char* tmp = (char*) malloc(sizeof(char)*(strlen(ID1)+1)) ;
    if (tmp == NULL) {//check if memory have been allocated correctly
        free(j) ;//free resources before returning
        return memoryProblem ;
    }
    //initialize the fields of the new jerry as required
    j->ID = tmp ;
    strcpy(j->ID , ID1) ;
    j->attNum = 0 ;
    j->attributes = NULL ;
    createOrigin(originP->home , originP->name , &(j->originP)) ;
    j->happiness = happiness1 ;
    *jerryPP = j ;
    return success ;
}

status createPlanet(char* name1 , float x , float y , float z , Planet** planetPP) {
    //check for input correctness
    if (planetPP == NULL || name1 == NULL)
        return failure ;
    Planet* p = (Planet*) malloc(sizeof(Planet)) ;
    if (p == NULL)//check if memory was allocated correctly
        return memoryProblem ;
    char* tmp = (char*) malloc(sizeof(char)*(strlen(name1) + 1)) ;
    if (tmp == NULL) {//check if memory was allocated correctly
        free(p) ;//free all resources before returning
        return memoryProblem ;
    }
    //initialize the fields of the new jerry as required
    p->name = tmp ;
    strcpy(p->name , name1) ;
    p->x = x ;
    p->y = y ;
    p->z = z ;
    *planetPP = p ;
    return success ;
}

status createAttribute(char* name1 , float value1 , PhysicalCharacteristics** attributePP) {
    //check for input correctness
    if (attributePP == NULL || name1 == NULL)
        return failure ;
    PhysicalCharacteristics* p = (PhysicalCharacteristics*) malloc(sizeof(PhysicalCharacteristics)) ;
    if (p == NULL)//check if memory was allocated correctly
        return memoryProblem ;
    char* tmp = (char*) malloc(sizeof(char)*(strlen(name1) + 1)) ;
    if (tmp == NULL) {//check if memory was allocated correctly
        free(p) ;//free resources before returning
        return memoryProblem ;
    }
    //initialize fields as required
    p->name = tmp ;
    strcpy(p->name , name1) ;
    p->value = value1 ;
    *attributePP = p ;
    return success ;
}

status createOrigin(Planet* planetP , char* dimension , Origin** originPP) {
    //check for input correctness
    if (originPP == NULL || planetP == NULL || dimension == NULL)
        return failure ;
    Origin* p = (Origin*) malloc(sizeof(Origin)) ;
    if (p == NULL)//check if memory was allocated correctly
        return memoryProblem ;
    char* tmp = (char*) malloc(sizeof(char)*(strlen(dimension) + 1)) ;
    if (tmp == NULL) {//check if memory was allocated correctly
        free(p) ; //free resources before returning
        return memoryProblem ;
    }
    //initialize fields accordingly
    p->name = tmp ;
    strcpy(p->name , dimension) ;
    p->home = planetP ;
    *originPP = p ;
    return success ;
}

bool checkIfExists(Jerry* j , char* name) {
    //loop through all the attributes of j and check if their name is equals to the given one
    for (int i = 0 ; i < j->attNum ; i ++) {
        if (strcmp(name , (j->attributes)[i]->name) == 0) {
            return true ;
        }
    }
    return false ; //if not found return False(0)
}

status addAttribute(Jerry* j , PhysicalCharacteristics* att) {
    PhysicalCharacteristics** tmp ; //used to save the current attributes pointer , in case we lose the pointer we use tmp
    if (j->attributes != NULL) {//already have attributes thus reallocate the array
        tmp = j->attributes ;
        j->attributes = (PhysicalCharacteristics**) realloc(tmp , (j->attNum+1) * sizeof(PhysicalCharacteristics*));
    }else {//if j doesn't have any attributes then initialize his attributes array
        tmp = j->attributes ;
        j->attributes = (PhysicalCharacteristics**) malloc((j->attNum+1) * sizeof(PhysicalCharacteristics*));
    }
    if (j->attributes == NULL) {//check if memory allocation failed
        j->attributes = tmp ;
        return memoryProblem ;
    }
    //update attributes of j accordingly
    (j->attributes)[j->attNum] = att ;
    j->attNum ++ ;
    return success ;
}

status deleteAtt(Jerry* j , char* name) {
    //check for input correctness
    if (j == NULL)
        return failure ;
    if(name == NULL)
        return failure ;
    if (!checkIfExists(j , name)) //check if the given attribute name already exist within j
        return failure ;
    int foundIndex = -1 ;
    //move all attributes which are after the given attribute one index back , thus
    //overriding the previous value .
    for (int i = 0 ; i < j->attNum ; i ++) {
        if (strcmp(j->attributes[i]->name,name) != 0) {
            if (foundIndex != -1)
                j->attributes[i - 1] = j->attributes[i] ;
        }else {//if the attribute is found then destroy it
            foundIndex = i ;
            destroyPhysicalCharacteristics(j->attributes[foundIndex]) ;
        }
    }
    j->attNum -- ;
    if (j->attNum == 0){
        j->attributes = NULL ;
        return success ;
    }
    //update j accordingly
    PhysicalCharacteristics** tmp = (PhysicalCharacteristics**) realloc(j->attributes , sizeof(PhysicalCharacteristics*) * (j->attNum));
    if (tmp == NULL) // check for allocation problems
        return memoryProblem ;
    j->attributes = tmp ;
    return success ;
}

//this function rounds a given number as defined in the assignment
static float roundTo2(float num) {
    float toRound = num * pow(10, 2) ;
    float result ;
    if (toRound >= 0) {
        result = (int) (toRound + 0.5) ;
    }else {
        result = (int) (toRound - 0.5) ;
    }
    return ((float) result) / pow(10, 2);
}

status printPlanet(Planet* planetP) {
    //input correctness check
    if (planetP == NULL)
        return failure ;
    if (planetP->name == NULL)
        return failure ;
    printf("Planet : %s (%.2f,%.2f,%.2f) \n" , planetP->name , roundTo2(planetP->x) , roundTo2(planetP->y) , roundTo2(planetP->z)) ;
    return success ;
}

status printJerry(Jerry* jerry) {
    //input correctness
    if (jerry == NULL)
        return failure ;
    if (jerry->originP == NULL)
        return failure ;
    printf("Jerry , ID - %s : \n" , jerry->ID) ;
    printf("Happiness level : %d \n" , jerry->happiness) ;
    printf("Origin : %s \n" , jerry->originP->name);
    if (printPlanet(jerry->originP->home) == failure)
        return failure ;
    if (jerry->attNum > 0){ //printing with a loop each attribute in a format defined in the assignment
        printf("Jerry's physical Characteristics available : \n\t") ;
        for (int i = 0 ; i < jerry->attNum - 1; i ++) {
            printf("%s : %.2f , " , jerry->attributes[i]->name , jerry->attributes[i]->value) ;
        }
        printf("%s : %.2f \n" , jerry->attributes[jerry->attNum-1]->name , jerry->attributes[jerry->attNum-1]->value) ;
    }
    return success ;
}


//destroy functions only destroy whatever was allocated in their corresponding create functions
void destroyPlanet(Planet* planet) {
    if (planet != NULL) {
        free(planet->name) ;
        free(planet) ;
    }
}

void destroyOrigin(Origin* origin) {
    if (origin != NULL) {
        free(origin->name);
        free(origin) ;
    }
}

void destroyPhysicalCharacteristics(PhysicalCharacteristics* p) {
    if (p != NULL) {
        free(p->name) ;
        free(p) ;
    }
}

void destroyJerry(Jerry* jerry) {
    if (jerry != NULL) {
        free(jerry->ID) ;
        destroyOrigin(jerry->originP) ; //an origin is being created in createJerry so destroy it here
        for (int i = 0 ; i < jerry->attNum ; i ++) {//destroy all attributes of jerry
            destroyPhysicalCharacteristics(jerry->attributes[i]) ;
        }
        free(jerry->attributes) ;
        free(jerry) ;
    }
}