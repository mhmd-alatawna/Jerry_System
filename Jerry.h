#ifndef JERRY_H
#define JERRY_H
#include "Defs.h"


typedef struct Planet1 Planet;
typedef struct Origin1 Origin;
typedef struct PhysicalCharacteristics1 PhysicalCharacteristics;
typedef struct Jerry1 Jerry;

//this struct is used to describe where a jerry came from , it is critical information about jerry
struct Planet1 {
    char* name;
    float x ,y ,z ;
};

//this struct is a wrapper of planet , which adds another information about jerry , dimension .
struct Origin1 {
    Planet* home;
    char* name;
};

//this struct is used as general information about a specific jerry , it adds information like
//his height , age and so on .
struct PhysicalCharacteristics1 {
    char* name;
    float value;
};

//this is the main struct of the program , this structs sums up all the information about
//one jerry so we can process and manage the jerries as required
struct Jerry1 {
    char* ID ;
    int happiness;
    Origin* originP;
    PhysicalCharacteristics** attributes;
    int attNum ;
};

//allocates memory for a new jerry and filling this jerry's fields according to the arguments provided
//memory is allocated if a copy is needed , like when coping a string provided as an argument
//after being done , the pointer of the new jerry will go into *jerryPP
status createJerry(char* ID1 , int happiness1 , Origin* originP , Jerry** jerryPP) ;

//allocats memory for a new planet and filling it accordingly by the giving arguments
//memory is allocated if a copy is needed , like when coping a string provided as an argument
//after being done , the pointer of the new planet will go into *planetPP
status createPlanet(char* name1 , float x , float y , float z , Planet** planetPP) ;

//allocats memory for a new physicalAttribute and filling it accordingly by the giving arguments
//memory is allocated if a copy is needed , like when coping a string provided as an argument
//after being done , the pointer of the new physicalAttribute will go into *attributePP
status createAttribute(char* name1 , float value1 , PhysicalCharacteristics** attributePP) ;

//allocats memory for a new Origin and filling it accordingly by the giving arguments
//memory is allocated if a copy is needed , like when coping a string provided as an argument
//after being done , the pointer of the new Origin will go into *originPP
status createOrigin(Planet* planetP , char* dimension , Origin** originPP) ;

//return true if the given jerry have an attribute with the same name as provided in the second argument
bool checkIfExists(Jerry* j , char* name) ;

//adds to a given jerry a given attribute , while accordingly reallocating memory
status addAttribute(Jerry* j , PhysicalCharacteristics* att) ;

//removing a given attribute from a given jerry , while accordingly reallocating memory
status deleteAtt(Jerry* j , char* name) ;

//prints all information about a given planet
status printPlanet(Planet* planetP) ;

//prints all information about a given jerry
status printJerry(Jerry* jerry) ;

//freeing any memory allocated from createPlanet function for the given planet which was created
//using the createPlanet function
void destroyPlanet(Planet* planet) ;

//freeing any memory allocated from createOrigin function for the given origin which was created
//using the createOrigin function
void destroyOrigin(Origin* origin) ;

//freeing any memory allocated from createPhysicalAttribute function for the given physicalAttribute which was created
//using the createPhysicalAttribute function
void destroyPhysicalCharacteristics(PhysicalCharacteristics* p) ;

//freeing any memory allocated from createJerry function for the given jerry which was created
//using the createJerry function
void destroyJerry(Jerry* jerry) ;
#endif
