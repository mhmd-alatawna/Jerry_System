#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Defs.h"
#include "HashTable.h"
#include "Jerry.h"
#include "LinkedList.h"
#include "MultiValueHashTable.h"

hashTable jerriesTable ; //this hashTable will save as keys (pointers to strings[ID]) and as values (pointers to jerry pointers[Jerry**])
hashTable planetsTable ; //this hashTable will save as keys (pointers to strings[name]) and as values (pointers to planet pointers[Planet**])
MultiHashTable jerriesAttributeTable ; //this hashTable will save as keys (pointers to strings[attName]) and as values (pointers to jerry pointers[Jerry**])
LinkedList planetsList ; //this linked list will save Planet** which will have all the planets in the system
LinkedList jerriesList ; //this linked list will save Jerry** which will have all the jerries in the system
int numberOfPlanets ;
int numberOfLinesInFile ;

//a function which knows how to print a string pointer key .
status printStringPKey(char** str) {
    printf("%s : \n", *str);
    return success ;
}

//this section provides functionality to handle string pointers
char** copyStringP(char** str) {
    char** result = (char**) malloc(sizeof(char*)) ;
    if (result == NULL) {
        return NULL ;
    }
    *result = (char*) malloc(sizeof(char) * (strlen(*str) + 1) ) ;
    if (*result == NULL) {
        return NULL ;
    }
    strcpy(*result, *str) ;
    return result ;
}
status freeStringP(char** str) {
    free(*str) ;
    free(str) ;
    return success ;
}
status printStringP(char** str) {
    printf("%s", *str) ;
    return success ;
}
bool isEqualStringP(char** str1, char** str2) {
    if (strlen(*str1) != strlen(*str2))
        return false ;
    return strcmp(*str1 , *str2) == 0 ;
}

//this section provides functionality to handle Planet pointers
Planet** copyPlanetPP(Planet** planetPP) {
    Planet** result = (Planet**) malloc(sizeof(Planet*)) ;
    *result = *planetPP ;
    return result ;
}
status freePlanetPP(Planet** planetPP) {
    free(planetPP) ;
    return success ;
}
status printPlanetPP(Planet** planet) {
    printPlanet(*planet) ;
    return success ;
}
bool isEqualPlanetPP(Planet** planetPP1 , Planet** planetPP2) {
    return *planetPP1 == *planetPP2 ;
}

//this section provides functionality to handle Jerry pointers
Jerry** copyJerryPP(Jerry** jerryPP) {
    Jerry** result = (Jerry**) malloc(sizeof(Jerry*)) ;
    *result = *jerryPP ;
    return result ;
}
status freeJerryPP(Jerry** jerryPP) {
    free(jerryPP) ;
    return success ;
}
status printJerryPP(Jerry** jerry) {
    printJerry(*jerry) ;
    return success ;
}
bool isEqualJerryPP(Jerry** jerry1 , Jerry** jerry2) {
    return *jerry1 == *jerry2 ;
}

//due to the construction of this program the last two sections are seperated although both of them
//are simply handling points which is the same functionality , this separation will allow the programmer to
//alter the program in an easier way .

//a function which receives a string pointer and sums it's characters ascii codes then return the result
//used as part of the hash function which required to initialize a hashTable/MultiValueHashTable
int transformStringToInt(char** str) {
    int result = 0 ;
    if (str == NULL) {
        return 0 ;
    }
    int index = 0;
    char curr = (*str)[index] ;
    while (curr != '\0') {
        result = result + (int)curr ;
        index ++ ;
        curr = (*str)[index] ;
    }
    return result ;
}



//a function that reads one line of a given file , this function returns a new allocated string , thus requires freeing
status readLine(FILE *file , char** a) {
    int size = 1 ;
    char* line = (char*) malloc(size*sizeof(char)); //initialized to size 1
    if (line == NULL) {
        return fileProblem ;
    }

    int len = 0;
    int ch = fgetc(file); //read one char
    //this loop basically keeps reading characters until it reaches end of line or end of file
    while (ch != EOF && ch != '\n') {
        size ++ ;
        line[len] = (char)ch;
        char* tmp = (char*) realloc(line, size);
        if (tmp == NULL) {
            free(line);
            return memoryProblem;
        }
        line = tmp;
        ch = fgetc(file) ;
        len ++ ;
    }

    line[len] = '\0'; //to maintain line as a null terminating string

    if (len == 0 && ch == EOF) { //check if we reached the end of the giving file
        free(line);
        return endOfFile;
    }

    *a = line ;//provide the address of the line to the giving pointer
    return success;
}

//this function receives a string and divide it into 'n' strings which will be saved in 'arrP'
//the function will start reading the string from 'start' and divide the string into sub-strings when encountering
//a ',' or ':' or '\0'
status extract(const char* str , int start , int n , char*** arrP) {
    int* lengthArr = (int*) malloc(sizeof(int)*n) ; //an array that will have the number of characters in each string we want to return
    if (lengthArr == NULL)
        return memoryProblem ;
    for (int i = 0 ; i < n ; i ++) {//initialize the values of lengthArr
        lengthArr[i] = 0 ;
    }

    //this loop will count how many characters are in each substring , and add the amounts to lengthArr
    int curr = start - 1 ;
    char ch ;
    for (int i = 0 ; i < n ; i ++) {
        curr ++ ;
        ch = str[curr] ;
        while (ch != ',' && ch != '\0' && ch != ':') {
            lengthArr[i] ++ ;
            curr ++ ;
            ch = str[curr] ;
        }
    }

    char** strArr = (char**) malloc(sizeof(char*)*n) ;
    if (strArr == NULL) {
        free(lengthArr) ;
        return memoryProblem ;
    }
    //allocate memory for each substring and if there is a problem then free all the allocated memory till now
    for (int i = 0 ; i < n ; i ++) {
        strArr[i] = (char*) malloc(sizeof(char)*(lengthArr[i] + 1)) ;
        if (strArr[i] == NULL) {
            for (int j = 0 ; j < i ; j ++) {
                free(strArr[j]) ;
            }
            free(strArr) ;
            free(lengthArr);
            return memoryProblem ;
        }
    }

    //read each character and build the substrings accordingly , memory already allocated thus
    //no problems will be caused
    curr = start - 1 ;
    for (int i = 0 ; i < n ; i ++) {
        curr ++ ;
        ch = str[curr] ;
        int l = 0 ;
        while (ch != ',' && ch != '\0' && ch != ':') {
            strArr[i][l] = ch ;
            curr ++ ;
            l ++ ;
            ch = str[curr] ;
        }
        strArr[i][l] = '\0' ;
    }

    free(lengthArr) ;
    *arrP = strArr ;
    return success ;
}

//given a string which represents a jerry ,create a new jerry with the information and return a pointer to it
Jerry* ExtractOneJerry(char* currLine) {
    char** arr ;
    char* error ;
    extract(currLine , 0 , 4 , &arr) ; //extract the parts of information representing the wanted jerry

    Jerry* jerryP ;
    Origin* originP ;
    Planet** tmp = lookupInHashTable(planetsTable , &(arr[2])); //find his planet by looking in the planetsTable by its name
    if (tmp == NULL)
        return NULL ;
    Planet* planetP = *tmp ;
    createOrigin(planetP , arr[1] , &originP) ;
    createJerry(arr[0] , (int) strtof(arr[3] , &error), originP , &jerryP) ; //create the jerry
    destroyOrigin(originP) ; //in jerry we create a copy of origin thus destroy this origin to prevent memory leaks

    //free all allocated memory from the extract function
    for (int h = 0 ; h < 4 ; h ++) {
        free(arr[h]) ;
    }
    free(arr) ;
    return jerryP ;
}

//given a string which represents an attribute , create a new attribute with the information and return a pointer to it
void ExtractOneAttribute(Jerry* jerryP , char* currLine) {
    char** arr1 ;
    char* error ;
    extract(currLine , 1 , 2 , &arr1) ;
    PhysicalCharacteristics* attP ;
    createAttribute(arr1[0] , strtof(arr1[1] , &error) , &attP) ;
    addAttribute(jerryP , attP) ;
    for (int h = 0 ; h < 2 ; h ++) {
        free(arr1[h]) ;
    }
    free(arr1) ;
}

//given a string which represents a planet , create a new planet with the information and return a pointer to it
Planet* ExtractOnePlanet(char* currLine) {
    Planet* planetP ;
    char** arr ;
    char* error ;
    extract(currLine , 0 , 4 , &arr) ;
    createPlanet(arr[0] , strtof(arr[1] , &error), strtof(arr[2] , &error) , strtof(arr[3] , &error) , &planetP) ;

    //free allocated memory from extract function
    for (int h = 0 ; h < 4 ; h ++) {
        free(arr[h]) ;
    }
    free(arr) ;
    return planetP ;
}

//adds a new planet to all the ADTs responsible to hold planets
status addPlanetToDataStructures(Planet* planetP) {
    status result ;
    if (planetP == NULL)
        return failure ;
    result = addToHashTable(planetsTable , &(planetP->name) , &planetP) ;
    if (result != success)
        return result;
    result = appendNode(planetsList , &planetP) ;
    if (result != success)
        return result;
    return success ;
}

//adds a new jerry to all the ADTs responsible to hold jerries
status addJerryToDataStructures(Jerry* jerryP) {
    status result ;
    if (jerryP == NULL) {
        return failure ;
    }
    for (int i = 0 ; i < jerryP->attNum ; i ++) {//add the new jerry to each attribute of his , to the attributesTable
        result = addToMultiHashTable(jerriesAttributeTable , &(jerryP->attributes[i]->name) , &jerryP) ;
        if (result != success)
            return result ;
    }
    result = addToHashTable(jerriesTable , &(jerryP->ID) , &jerryP) ;
    if (result != success)
        return result ;
    result = appendNode(jerriesList , &jerryP) ;
    if (result != success)
        return result ;
    return success ;
}

//read the jerries part from the configuration file and update the ADTs accordingly
status readJerries(FILE* configurationFile) {
    status result ;
    char* currLine ;

    //read title and ignore it
    result = readLine(configurationFile , &currLine) ;
    if (result == memoryProblem)
        return memoryProblem ;
    if (result == fileProblem)
        return fileProblem ;
    free(currLine) ;
    currLine = NULL ;


    //read the first jerry , if not exists then the result will receive endOfFile
    result = readLine(configurationFile , &currLine) ;
    if (result == memoryProblem)
        return memoryProblem ;
    if (result == fileProblem)
        return fileProblem ;
    if (result == endOfFile) {
        return success ;
    }

    //for each jerry we will read his information and then the few lines after his information
    //which will contain his attributes , we determine that the attributes are done when encountering a non tab start .
    while (result != endOfFile) {
        Jerry* jerryP = ExtractOneJerry(currLine) ; //read the current jerry
        if (jerryP == NULL) {
            free(currLine) ;
            return failure ;
        }

        free(currLine) ;
        currLine = NULL ;

        result = readLine(configurationFile , &currLine) ; //fill currLine for the while which will check if its an attribute
        if (result == memoryProblem)
            return memoryProblem ;
        if (result == fileProblem)
            return fileProblem ;
        if (result == endOfFile)
            break;

        while (currLine[0] == '\t') {//while the current line is an attribute , create one and add it to the current jerry
            ExtractOneAttribute(jerryP , currLine) ;

            free(currLine) ;
            currLine = NULL ;

            result = readLine(configurationFile , &currLine) ;
            if (result == memoryProblem)
                return memoryProblem ;
            if (result == fileProblem)
                return fileProblem ;
            if (result == endOfFile)
                break;
        }
        status result2 = addJerryToDataStructures(jerryP) ;
        if (result2 != success)
            return result2 ;
    }
    return success ;
}

//read the planets part from the configuration file and update the ADTs accordingly
status readPlanets(FILE* configurationFile) {
    //start reading from the file , after we are done with each line , we free the memory that saved the line
    char* currLine ;
    status result = readLine(configurationFile , &currLine) ;
    if (result == memoryProblem)
        return memoryProblem ;
    if (result == fileProblem)
        return fileProblem ;
    free(currLine) ;
    currLine = NULL ;

    //first section will contain the planets and this loop will create the planets using functions
    //explained before , after the creation and updating planets array we made sure to free any used unnecessary memory
    for (int i = 0 ; i < numberOfPlanets ; i ++) {
        result = readLine(configurationFile , &currLine) ;
        if (result == memoryProblem)
            return memoryProblem ;
        if (result == fileProblem)
            return fileProblem ;

        Planet* planetP = ExtractOnePlanet(currLine) ;
        free(currLine) ;
        currLine = NULL ;
        status result2 = addPlanetToDataStructures(planetP) ;
        if (result2 != success)
            return result2 ;
    }
    return success ;
}

//counts number of lines in the configuration file to create a good hashNumber
static status countNumberOfLinesInFile(char* fileName) {
    numberOfLinesInFile = 0 ;
    FILE* configurationFile = fopen(fileName,"r") ;//open the given file so we can read from it
    if (configurationFile == NULL)
        return fileProblem ;

    char* currLine ;
    status result = readLine(configurationFile , &currLine) ;
    if (result == memoryProblem)
        return memoryProblem ;
    while (result != endOfFile) {
        free(currLine) ;
        result = readLine(configurationFile , &currLine) ;
        if (result == memoryProblem)
            return memoryProblem;
        numberOfLinesInFile ++ ;
    }
    fclose(configurationFile) ;
    return success ;
}

//this function will initialize all the ADTs to empty ones and provide them with the necessarily functionality
status init(int argc , char* argv[]) {
    //input correctness checks
    if (argc != 3)
        return failure ;
    char* error ;
    numberOfPlanets = strtol(argv[1], &error, 10); //extract the integer from args
    if (numberOfPlanets < 0)
        return failure ;
    countNumberOfLinesInFile(argv[2]) ;
    planetsList = createLinkedList(copyPlanetPP , freePlanetPP , printPlanetPP , isEqualPlanetPP , NULL) ;
    jerriesList = createLinkedList(copyJerryPP , freeJerryPP , printJerryPP , isEqualJerryPP , NULL) ;
    planetsTable = createHashTable(copyStringP , freeStringP , printStringP , copyPlanetPP , freePlanetPP , printPlanetPP , isEqualStringP , transformStringToInt , numberOfPlanets) ;
    jerriesTable = createHashTable(copyStringP , freeStringP , printStringP , copyJerryPP , freeJerryPP , printJerryPP , isEqualStringP , transformStringToInt , numberOfLinesInFile) ;
    jerriesAttributeTable = createMultiHashTable(copyStringP , freeStringP , printStringPKey , isEqualStringP , copyJerryPP , freeJerryPP , printJerryPP , isEqualJerryPP, transformStringToInt , numberOfLinesInFile) ;
    return success ;
}

//this function will read the full configuration file and fill the ADTs accordingly
status readFileAndFillADTs(int argc , char* argv[]) {
    FILE* configurationFile = fopen(argv[2],"r") ;//open the given file so we can read from it
    if (configurationFile == NULL)
        return fileProblem ;

    if (readPlanets(configurationFile) != success)
        return failure ;
    if (readJerries(configurationFile) != success)
        return failure ;

    fclose(configurationFile) ;
    return success ;
}

//receives from the user a string input and allocate memory for it then put the pointer to this memory in *str
status getString(char** str) {
    char* tmp = (char*) malloc(sizeof(char) * 301); // max user input size
    if (tmp == NULL)
        return memoryProblem ;
    scanf("%s" , tmp) ; //read input
    int x = 0 ; //calculate the length of the rechieved input
    char ch = tmp[x] ;
    while (ch != '\0') {
        x ++ ;
        ch = tmp[x] ;
    }
    char* result = (char*) malloc(sizeof(char) * (x+1)) ; //allocate an exact needed memory to store the input
    if (result == NULL) {
        free(tmp) ;
        return memoryProblem ;
    }
    for (int i = 0 ; i < x + 1 ; i ++) {//copy the input into the new allocated memory in the heap
        result[i] = tmp[i] ;
    }
    free(tmp) ;//free the used temporary buffer
    *str = result ; //update the given pointer to point to the user input
    return success;
}

//this function will clean and destroy all the ADTs thus preventing any memory leaks at the end of the program
//note that all ADTs hold pointers to pointers to the objects thus the destroy functions of each ADT doesn't
//destroy planets/jerries/attributes/origins , we will destroy them using a for loop on the LinkedLists of jerries and planets
void clean() {
    destroyHashTable(jerriesTable) ;
    destroyHashTable(planetsTable) ;
    destroyMultiHashTable(jerriesAttributeTable) ;
    for (int i = 1 ; i <= getLengthList(planetsList) ; i ++) {
        destroyPlanet(*((Planet**)getDataByIndex(planetsList , i))) ;
    }
    for (int i = 1 ; i <= getLengthList(jerriesList) ; i ++) {
        destroyJerry(*((Jerry**)getDataByIndex(jerriesList , i))) ;
    }
    destroyList(planetsList) ;
    destroyList(jerriesList) ;
}

//print the main menu
void printMenu() {
    printf("Welcome Rick, what are your Jerry's needs today ? \n");
    printf("1 : Take this Jerry away from me \n");
    printf("2 : I think I remember something about my Jerry \n");
    printf("3 : Oh wait. That can't be right \n");
    printf("4 : I guess I will take back my Jerry now \n");
    printf("5 : I can't find my Jerry. Just give me a similar one \n");
    printf("6 : I lost a bet. Give me your saddest Jerry \n");
    printf("7 : Show me what you got \n");
    printf("8 : Let the Jerries play \n");
    printf("9 : I had enough. Close this place \n");
}

//this function will ask the user to pick an option from the main menu and check if its legal , if
//not then the function will keep asking the user for a legal input while printing appropriate messages
int getOption() {
    char* input ;
    char* inputError ;
    if (getString(&input) == memoryProblem) { //get input from user
        printf("Memory problem") ;
        clean() ;
        return -1 ;
    }
    int option = strtol(input, &inputError, 10);//extract the number from the input , if failed we will check inputError
    while ((inputError != NULL && (inputError[0] != '\n' && inputError[0] != '\0')) || (option < 1 || option > 9)) {
        free(input) ;//free after using resource
        while (getchar() != '\n') {}
        printf("Rick this option is not known to the daycare ! \n");
        printMenu();
        if (getString(&input) == memoryProblem) {
            printf("Memory problem") ;
            clean() ;
            return -1 ;
        }
        option = strtol(input, &inputError, 10) ;//extract the number from the input , if failed we will check inputError
    }
    free(input) ;//free after using resource
    return option ;
}

//this function represents the first option , which will ask the user for details about a jerry and add it
//to the system
status takeJerryAwayFromRick() {
    printf("What is your Jerry's ID ? \n") ;
    char* ID ;
    if (getString(&ID) == memoryProblem) //get the id from the user
        return memoryProblem ;
    Jerry* jerryP = lookupInHashTable(jerriesTable , &ID) ; //find the corresponding jerry
    if (jerryP != NULL) { // if found
        free(ID) ;
        printf("Rick did you forgot ? you already left him here ! \n") ;
        return inputFailure ;
    }

    printf("What planet is your Jerry from ? \n") ; //ask the user for a planet name
    char* planetName ;
    if (getString(&planetName) == memoryProblem) {//get the planet name
        free(ID) ;
        return memoryProblem ;
    }

    Planet* planetP = lookupInHashTable(planetsTable , &planetName);
    if (planetP == NULL) {//if the planet not found
        printf("%s is not a known planet ! \n" , planetName) ;
        free(ID) ;
        free(planetName) ;
        return inputFailure ;
    }
    planetP = (*((Planet**)planetP)); //correct the planet pointer (the ADT holds Planet**)


    printf("What is your Jerry's dimension ? \n") ;
    char* dimensionName ;
    if (getString(&dimensionName) == memoryProblem) {//ask for the dimention of the jerry
        free(ID) ;
        free(planetName) ;
        return memoryProblem ;
    }

    printf("How happy is your Jerry now ? \n") ;
    char* tmp ;
    if (getString(&tmp) == memoryProblem) {//get the happiness from the user as a string
        free(ID) ;
        free(planetName) ;
        free(dimensionName) ;
        return memoryProblem ;
    }

    char* error ;
    int value = strtol(tmp, &error , 10); //extract the float from the provided string
    if (error[0] != '\0') {//if the string doesn't hold a float
        free(ID) ;
        free(planetName) ;
        free(dimensionName) ;
        free(tmp) ;
        return inputFailure ;
    }
    free(tmp) ;//free used memory to save the string

    Origin* originP ;
    if (createOrigin(planetP , dimensionName , &originP) == memoryProblem) {//create the origin using the collected data
        free(ID) ;
        free(planetName) ;
        free(dimensionName) ;
        return memoryProblem ;
    }
    free(dimensionName) ;
    free(planetName) ;

    Jerry* jerryToAdd ;
    if (createJerry(ID , value , originP , &jerryToAdd) == memoryProblem) {//create a new jerry accordingly
        free(ID) ;
        destroyOrigin(originP) ;
        return memoryProblem ;
    }
    destroyOrigin(originP) ;//destroy because a copy is made in jerry
    free(ID) ;

    printJerry(jerryToAdd) ; //print jerry
    return addJerryToDataStructures(jerryToAdd) ; //update ADTs
}

//second option in the main menu , asks the user for details to add a new attribute to a wanted jerry
status addNewAttributeForJerry() {
    printf("What is your Jerry's ID ? \n") ;
    char* ID ;
    if (getString(&ID) == memoryProblem) //get the id from the user
        return memoryProblem ;
    Jerry* jerryP = lookupInHashTable(jerriesTable , &ID) ; //find the corresponding jerry
    if (jerryP == NULL) {
        free(ID) ;
        printf("Rick this Jerry is not in the daycare ! \n") ;
        return inputFailure ;
    }
    jerryP = (*((Jerry**)jerryP)) ;//correct the jerry which is received as (Jerry**)

    printf("What physical characteristic can you add to Jerry - %s ? \n" , ID) ;
    char* attName ;
    if (getString(&attName) == memoryProblem) {//get the attribute's name
        free(ID) ;
        return memoryProblem ;
    }
    if (checkIfExists(jerryP , attName)) {//check if the attribute already exists
        printf("The information about his %s already available to the daycare ! \n" , attName) ;
        free(ID) ;
        free(attName) ;
        return inputFailure ;
    }

    printf("What is the value of his %s ? \n" , attName) ;
    char* tmp ;
    if (getString(&tmp) == memoryProblem) {//get the value from the user as a string
        free(ID) ;
        free(attName) ;
        return memoryProblem ;
    }
    char* error ;
    float value = strtof(tmp, &error); //extract the value from the provided string
    if (error[0] != '\0') {
        free(ID) ;
        free(attName) ;
        free(tmp) ;
        return failure ;
    }
    free(tmp) ;

    PhysicalCharacteristics* p ;
    if (createAttribute(attName , value , &p) == memoryProblem) {//create a new attribute accordingly
        free(ID) ;
        free(attName) ;
        return memoryProblem ;
    }
    if (addAttribute(jerryP , p) == memoryProblem) {//add the new attribute to the found jerry
        destroyPhysicalCharacteristics(p) ;
        free(ID) ;
        free(attName) ;
        return memoryProblem ;
    }
    //update the attributesTable accordingly
    if (addToMultiHashTable(jerriesAttributeTable , &attName , &jerryP) != success)
        return failure ;
    //print all jerries with the added attribute as required
    if (displayMultiValueHashElementsByKey(jerriesAttributeTable , &attName) != success)
        return failure ;
    //free resources after being done with them
    free(ID) ;
    free(attName) ;

    return success ;
}

//this is corresponding to option 3 , ask the user about a jerry and an attribute to remove then remove the attribute from the jerry
status removeAnAttributeFromJerry() {
    printf("What is your Jerry's ID ? \n") ;
    char* ID ;
    if (getString(&ID) == memoryProblem) //get the id from the user
        return memoryProblem ;
    Jerry* jerryP = lookupInHashTable(jerriesTable , &ID) ; //find the corresponding jerry
    if (jerryP == NULL) {
        free(ID) ;
        printf("Rick this Jerry is not in the daycare ! \n") ;
        return inputFailure ;
    }
    jerryP = (*((Jerry**)jerryP)) ; //correct the received jerry (from Jerry** to Jerry*)

    printf("What physical characteristic do you want to remove from Jerry - %s ? \n" , ID) ;
    char* attName ;
    if (getString(&attName) == memoryProblem) {//get the attribute's name
        free(ID) ;
        return memoryProblem ;
    }
    if (!checkIfExists(jerryP , attName)) {//check if the attribute doesn't exist
        printf("The information about his %s not available to the daycare ! \n" , attName) ;
        free(ID) ;
        free(attName) ;
        return inputFailure ;
    }

    if (deleteAtt(jerryP , attName) == memoryProblem) {//delete the attribute from the jerry
        free(ID) ;
        free(attName) ;
        return memoryProblem ;
    }
    //update the attributesTable accordingly
    if (removeFromMultiHashTable(jerriesAttributeTable , &attName , &jerryP) != success) {
        free(ID) ;
        free(attName) ;
        return failure ;
    }

    printJerry(jerryP) ;

    //free resources after being done with them
    free(ID) ;
    free(attName) ;
    return success ;
}

// a function that removes a given jerry from all the corresponding ADTs
static status removeJerryFromAllDataStructures(Jerry* jerryP) {
    if (removeFromHashTable(jerriesTable , &(jerryP->ID)) != success) {
        return failure ;
    }
    //update the attributesTable
    for (int i = 0 ; i < jerryP->attNum ; i ++) {
        if (removeFromMultiHashTable(jerriesAttributeTable , &(jerryP->attributes[i]->name) , &jerryP) != success) {
            return failure ;
        }
    }
    if (deleteNode(jerriesList , &jerryP) != success) {
        return failure ;
    }
    destroyJerry(jerryP) ;
    return success ;
}

//corresponding to option 4 , gets information about a jerry and remove it from the system
status removeAJerry() {
    printf("What is your Jerry's ID ? \n") ;
    char* ID ;
    if (getString(&ID) == memoryProblem)//get ID
        return memoryProblem ;

    Jerry* jerryP = lookupInHashTable(jerriesTable , &ID) ; //find the corresponding jerry
    if (jerryP == NULL) {
        free(ID) ;
        printf("Rick this Jerry is not in the daycare ! \n") ;
        return inputFailure ;
    }
    jerryP = (*((Jerry**)jerryP)) ; // correct the jerry

    if (removeJerryFromAllDataStructures(jerryP) != success) { //update all the ADTs to remove the jerry
        free(ID) ;
        return failure ;
    }
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n") ;
    free(ID) ;
    return success ;
}

//does the mathematical absolute value
static float absuloteValue(float x) {
    if (x >= 0)
        return x ;
    return -x ;
}

//looks for the jerry which have the closest value of a giving attribute to a giving value ,
//the check jerries are inside jerriesToPickFrom
static Jerry* getCloserValueJerry(LinkedList jerriesToPickFrom , char* attName , float value) {
    Jerry* jerryP = getDataByIndex(jerriesToPickFrom , 1); //get first jerry , jerryP will be the best jerry for now
    if (jerryP == NULL) {
        free(attName) ;
        return NULL ;
    }
    jerryP = (*((Jerry**)jerryP)) ; //correct the jerry

    float bestValue = -1 ;
    for (int j = 0 ; j < jerryP->attNum ; j ++) {
        if (strcmp(jerryP->attributes[j]->name , attName) == 0)//check whats the value of the attribute of this jerry
            bestValue = absuloteValue(jerryP->attributes[j]->value - value) ;
    }
    for (int i = 2 ; i <= getLengthList(jerriesToPickFrom) ; i ++) {
        Jerry* currP = getDataByIndex(jerriesToPickFrom , i);//current checked jerry
        if (currP == NULL) {
            free(attName) ;
            return NULL ;
        }
        currP = (*((Jerry**)currP)) ;

        float currValue = 0 ;
        for (int j = 0 ; j < jerryP->attNum ; j ++) {//find the current jerry's attribute value
            if (strcmp(currP->attributes[j]->name , attName) == 0)
                currValue = absuloteValue(currP->attributes[j]->value - value) ;
        }
        if (currValue < bestValue) {//if its better that what is saved for now then update accordingly
            jerryP = currP ;
            bestValue = currValue ;
        }
    }
    return jerryP ;
}

//corresponding to option 5 , gets from the user an attribute name and a value and removes the
//jerry which have the closest value to the provided one .
status removeJerryByAttribute() {
    printf("What do you remember about your Jerry ? \n") ;
    char* attName ;
    if (getString(&attName) == memoryProblem)//get attribute name
        return memoryProblem ;

    LinkedList toLookUp = lookUpInMultiHashTable(jerriesAttributeTable , &attName) ;//find if any of the jerries have this attribute
    if (toLookUp == NULL) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n" , attName) ;
        free(attName) ;
        return inputFailure ;
    }

    printf("What do you remember about the value of his %s ? \n" , attName) ;
    char* tmp ;
    if (getString(&tmp) == memoryProblem) {//get the value from the user as a string
        free(attName) ;
        return memoryProblem ;
    }
    char* error ;
    float value = strtof(tmp, &error); //extract the value from the provided string
    if (error[0] != '\0') {
        free(attName) ;
        free(tmp) ;
        return failure ;
    }
    free(tmp) ;

    Jerry* jerryP = getCloserValueJerry(toLookUp , attName , value) ; //get closest jerry
    printf("Rick this is the most suitable Jerry we found : \n") ;
    printJerry(jerryP) ;
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n") ;
    free(attName) ;
    return removeJerryFromAllDataStructures(jerryP) ; //remove the jerry
}

//corresponding to option 6 , finds the jerry with the smallest happiness and removes it
status giveRickTheSaddestJerry() {
    if (getLengthList(jerriesList) == 0) {//if no jerries are in the system then return .
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n") ;
        return inputFailure ;
    }
    Jerry* jerryP = getDataByIndex(jerriesList, 1); //get first jerry and jerryP will hold the current min
    if (jerryP == NULL) {
        return failure ;
    }
    jerryP = (*((Jerry**)jerryP)) ;

    for (int i = 2 ; i <= getLengthList(jerriesList) ; i ++) {
        Jerry* currP = getDataByIndex(jerriesList , i) ;//get current jerry
        if (currP == NULL) {
            return failure ;
        }
        currP = (*((Jerry**)currP)) ;
        if (currP->happiness < jerryP->happiness) {//if the current jerry is lower than the saved one then update accordingly
            jerryP = currP ;
        }
    }
    printf("Rick this is the most suitable Jerry we found : \n") ;
    if (printJerry(jerryP) != success)
        return failure ;
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n") ;
    removeJerryFromAllDataStructures(jerryP) ;//remove the printed jerry
    return success ;
}

//print the menu corresponding to option 7
void printMiniMenu1() {
    printf("What information do you want to know ? \n");
    printf("1 : All Jerries \n");
    printf("2 : All Jerries by physical characteristics \n");
    printf("3 : All known planets \n");
}

//print the menu corresponding to option 8
void printMiniMenu2() {
    printf("What activity do you want the Jerries to partake in ? \n");
    printf("1 : Interact with fake Beth \n");
    printf("2 : Play golf \n");
    printf("3 : Adjust the picture settings on the TV \n");
}

//prints all jerries in the given LinkedList
status printAllJerries(LinkedList listToPrint) {
    if (getLengthList(listToPrint) == 0) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n") ;
        return inputFailure ;
    }
    for (int i = 1 ; i <= getLengthList(listToPrint) ; i ++) {
        Jerry* currP = getDataByIndex(listToPrint , i) ;
        if (currP == NULL)
            return failure ;
        currP = (*((Jerry**)currP)) ;
        if (printJerry(currP) != success)
            return failure ;
    }
    return success ;
}

//prints all planets in the given LinkedList
status printAllPlanets(LinkedList listToPrint) {
    if (getLengthList(listToPrint) == 0) {
        return inputFailure ;
    }
    for (int i = 1 ; i <= getLengthList(listToPrint) ; i ++) {
        Planet* currP = getDataByIndex(listToPrint , i) ;
        if (currP == NULL)
            return failure ;
        currP = (*((Planet**)currP)) ;
        if (printPlanet(currP) != success)
            return failure ;
    }
    return success ;
}

//corresponding to option 7.1 ,  prints all jerries with the given attribute
status printAllJerriesByAttribute(char* attName) {
    LinkedList toPrint = lookUpInMultiHashTable(jerriesAttributeTable , &attName) ;
    if (toPrint == NULL) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n" , attName) ;
        return inputFailure ;
    }
    printf("%s : \n" , attName ) ;
    return printAllJerries(toPrint) ;
}

//option 7
status printReportsForRick() {
    printMiniMenu1() ;
    char* input ;
    char* inputError ;
    if (getString(&input) == memoryProblem) { //get input from user
        printf("Memory problem") ;
        clean() ;
        return memoryProblem ;
    }
    int option = strtol(input, &inputError, 10);//extract the number from the input , if failed we will check inputError
    if ((inputError != NULL && (inputError[0] != '\n' && inputError[0] != '\0')) || (option < 1 || option > 3)) {
        printf("Rick this option is not known to the daycare ! \n") ;
        free(input) ;
        return inputFailure ;
    }
    free(input) ;

    status result ;
    if (option == 1) {
        return printAllJerries(jerriesList) ;
    }else if (option == 2) {
        printf("What physical characteristics ? \n") ;
        char* attName ;
        if (getString(&attName) == memoryProblem)//ask for attribute name
            return memoryProblem ;
        result = printAllJerriesByAttribute(attName) ;
        free(attName) ;
        return result ;
    }else {
        return printAllPlanets(planetsList) ;
    }

}

//update all jerries's happiness to +15 if >= 20 otherwise -5 , while maintaning the happiness in the [0,100] range
status interactWithFakeBeth() {
    for (int i = 1 ; i <= getLengthList(jerriesList) ; i ++) {
        Jerry* currP = getDataByIndex(jerriesList , i) ;
        if (currP == NULL)
            return failure ;
        currP = (*((Jerry**)currP)) ;
        if (currP->happiness >= 20) {
            currP->happiness += 15 ;
            if (currP->happiness > 100)
                currP->happiness = 100 ;
        }else {
            currP->happiness -= 5 ;
            if (currP->happiness < 0)
                currP->happiness = 0 ;
        }
    }
    return success ;
}

//update all jerries's happiness to +10 if >= 50 otherwise -10 , while maintaning the happiness in the [0,100] range
status playGolf() {
    for (int i = 1 ; i <= getLengthList(jerriesList) ; i ++) {
        Jerry* currP = getDataByIndex(jerriesList , i) ;
        if (currP == NULL)
            return failure ;
        currP = (*((Jerry**)currP)) ;
        if (currP->happiness >= 50) {
            currP->happiness += 10 ;
            if (currP->happiness > 100)
                currP->happiness = 100 ;
        }else {
            currP->happiness -= 10 ;
            if (currP->happiness < 0)
                currP->happiness = 0 ;
        }
    }
    return success ;
}

//update all jerries's happiness to +20 , while maintaning the happiness in the [0,100] range
status adjustTvSettings() {
    for (int i = 1 ; i <= getLengthList(jerriesList) ; i ++) {
        Jerry* currP = getDataByIndex(jerriesList , i) ;
        if (currP == NULL)
            return failure ;
        currP = (*((Jerry**)currP)) ;
        currP->happiness += 20 ;
        if (currP->happiness > 100)
            currP->happiness = 100 ;
    }
    return success ;
}


//option 8
status makeJerriesHappier() {
    if (getLengthList(jerriesList) == 0) {//if no jerries are in the system
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n") ;
        return inputFailure ;
    }

    printMiniMenu2() ;
    char* input ;
    char* inputError ;
    if (getString(&input) == memoryProblem) { //get input from user
        printf("Memory problem") ;
        clean() ;
        return memoryProblem ;
    }
    int option = strtol(input, &inputError, 10);//extract the number from the input , if failed we will check inputError
    if ((inputError != NULL && (inputError[0] != '\n' && inputError[0] != '\0')) || (option < 1 || option > 3)) {
        printf("Rick this option is not known to the daycare ! \n") ;
        free(input) ;
        return inputFailure ;
    }
    free(input) ;

    status result ;
    if (option == 1) {
        result = interactWithFakeBeth() ;
    }else if (option == 2) {
        result = playGolf() ;
    }else {
        result = adjustTvSettings() ;
    }
    printf("The activity is now over ! \n") ;
    printAllJerries(jerriesList) ;
    return result ;
}

void main(int argc , char* argv[]) {
    MultiHashTable jerriesAttributeTable ;
    hashTable jerriesTable ;
    hashTable planetsTable ;
    LinkedList planetsList ;
    LinkedList jerriesList ;

    init(argc , argv) ; //initialize to empty ADTs
    readFileAndFillADTs(argc, argv) ;// fill ADTs according to the configuration file

    status result = failure ;
    while (true) {
        printMenu() ;
        int option = getOption(1 , 9) ; //aks repeatedly for input when incorrect
        if (option == -1) {//if error have been found
            clean() ;
            exit(-1) ;
        }
        if (option == 1) {
            result = takeJerryAwayFromRick() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 2){
            result = addNewAttributeForJerry() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program ") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 3) {
            result = removeAnAttributeFromJerry() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program ") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 4) {
            result = removeAJerry() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program ") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 5) {
            result = removeJerryByAttribute() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program ") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 6) {
            result = giveRickTheSaddestJerry() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program ") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 7) {
            result = printReportsForRick() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program ") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 8) {
            result = makeJerriesHappier() ;
            if (result == memoryProblem || result == failure) {
                printf("A memory problem has been detected in the program ") ;
                clean() ;
                exit(-1) ;
            }
        }else if (option == 9) {
            printf("The daycare is now clean and close ! \n") ;
            clean() ;
            exit(0) ;
        }
    }
}



