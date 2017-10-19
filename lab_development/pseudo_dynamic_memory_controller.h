/**
* pseudo_dynamic_memory_controller.h
* Purpose: create and control a heap.
*
* To do: 
*	Create multi-type PseudoDynamicMemoryController (change class to class template);
*	function "~PseudoDynamicMemoryController()";
*	function "void releaseMemory()".
*
* @author Edward Drozdov
* @version 1.0d 17/10/17
*/

#pragma once
#include <stdlib.h>
#include <iostream>
#include "exception_generator.h"

#define MEMORY_TYPE double
#define STRING_FORMAT "%lf"
#define STRING_OUTPUT_FORMAT "%.15le\n"


class PseudoDynamicMemoryController {
public:
	PseudoDynamicMemoryController();

	int initilizeMemory(int requiredMemorySize);

	MEMORY_TYPE* holdMemory(int quantity);
	MEMORY_TYPE* holdMemory(int quantity, MEMORY_TYPE initialValue);	

	MEMORY_TYPE* loadFile(char* readingFileName, int columnsTotal, int stringsTotal);
	int loadInfo(char* readingFileName, int* dimension, int* lowBandWidth);

	void saveInFile(char* writingFileName, MEMORY_TYPE* pointer, int quantity, int stringLength);

	void printMemoryUsage();

	void setAlertLevel(int newAlertLevel);

	int printMemoryContent(int startElementNumber, int endElementNumber, int stringLength);
	int printMemoryContent(MEMORY_TYPE* pointer, int quantity, int stringLength);
	int printMemoryContent(MEMORY_TYPE* pointer, int quantity, int stringLength, char* header);
	
	inline MEMORY_TYPE* getMemoryPoolPointer() { return memoryPoolPointer; };

private:
	int memoryLeft;
	int shift;
	MEMORY_TYPE* memoryPoolPointer;
	ExceptionGenerator exceptionGenerator;
};