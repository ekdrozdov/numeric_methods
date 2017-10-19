/**
* pseudo_dynamic_memory_controller.h
* Purpose: create and control a heap.
*
* To do: 
*	Create multi-type PseudoDynamicMemoryController;
*	function "~PseudoDynamicMemoryController()";
*	function "void releaseMemory()".
*
* @author Edward Drozdov
* @version 1.0d 17/10/17
*/

#pragma once
#include <stdlib.h>
#include <iostream>

#define MEMORY_TYPE double
#define STRING_FORMAT "%lf"
#define ERROR_INFO_STRING_LENGTH 70
#define WARNING_INFO_STRING_LENGTH 70


class PseudoDynamicMemoryController {
public:
	PseudoDynamicMemoryController();

	int initilizeMemory(int requiredMemorySize);
	inline MEMORY_TYPE* getMemoryPoolPointer() { return memoryPoolPointer; };

	MEMORY_TYPE* holdMemory(int quantity);
	MEMORY_TYPE* holdMemory(int quantity, MEMORY_TYPE initialValue);	
	MEMORY_TYPE* loadFile(char* readingFileName, int columnsTotal, int stringsTotal);
	int loadInfo(char* readingFileName, int* dimension, int* lowBandWidth);

	inline int getErrorsCount() { return errorsCount; };
	inline char* getErrorInfo() { return *errorInfo; };

	int printMemoryContent(int startElementNumber, int endElementNumber, int stringLength);
	int printMemoryContent(MEMORY_TYPE* pointer, int quantity, int stringLength);
	int printMemoryContent(MEMORY_TYPE* pointer, int quantity, int stringLength, char* header);

	void printMemoryUsage();

	void printErrorInfo();
	void printWarningInfo();

private:
	int memoryLeft;
	int shift;
	MEMORY_TYPE* memoryPoolPointer;

	int errorsCount;
	int warningsCount;
	char* errorInfo[ERROR_INFO_STRING_LENGTH];
	char* warningInfo[WARNING_INFO_STRING_LENGTH];
};