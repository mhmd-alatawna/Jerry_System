#ifndef DEFS_H_
#define DEFS_H_

typedef enum e_bool { false, true } bool;
typedef enum e_status { success, failure , memoryProblem , argumentsProblem , alreadyExists , fileProblem , endOfFile , inputFailure} status;

typedef void * Element;

typedef Element(*CopyFunction) (Element);
typedef status(*FreeFunction) (Element);
typedef status(*PrintFunction) (Element);
typedef int(*TransformIntoNumberFunction) (Element);
typedef bool(*EqualFunction) (Element, Element);

#endif /* DEFS_H_ */
