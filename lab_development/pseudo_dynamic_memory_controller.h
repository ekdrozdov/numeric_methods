/**
* pseudo_dynamic_memory_controller.h
* Purpose: create and control a heap.
*
* To do: 
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


template <typename MemoryType>
class PseudoDynamicMemoryController {
public:
	PseudoDynamicMemoryController();
	~PseudoDynamicMemoryController();

	int initilizeMemory(int requiredMemorySize);

	MemoryType* holdMemory(int quantity);
	MemoryType* holdMemory(int quantity, MemoryType initialValue);	

	MemoryType* loadFile(char* readingFileName, int columnsTotal, int stringsTotal);
	int loadInfo(char* readingFileName, int* dimension, int* lowBandWidth);

	void saveInFile(char* writingFileName, MemoryType* pointer, int quantity, int stringLength);

	void printMemoryUsage();
	void finalize();

	void setAlertLevel(int newAlertLevel);

	int printMemoryContent(int startElementNumber, int endElementNumber, int stringLength);
	int printMemoryContent(MemoryType* pointer, int quantity, int stringLength);
	int printMemoryContentFormat(MemoryType* pointer, int quantity, int stringLength);
	int printMemoryContent(MemoryType* pointer, int quantity, int stringLength, char* header);
	
	
	inline MemoryType* getMemoryPoolPointer() { return memoryPoolPointer; };

private:
	int memoryLeft;
	int shift;
	MemoryType* memoryPoolPointer;
	ExceptionGenerator exceptionGenerator;
};


using namespace std;

template<typename MemoryType> 
PseudoDynamicMemoryController<MemoryType>::PseudoDynamicMemoryController() {
	shift = 0;
	memoryLeft = 0;
};


template<typename MemoryType>
PseudoDynamicMemoryController<MemoryType>::~PseudoDynamicMemoryController() {
	shift = 0;
	memoryLeft = 0;
	free(memoryPoolPointer);
	memoryPoolPointer = NULL;
};


template<typename MemoryType>
int PseudoDynamicMemoryController<typename MemoryType>::
initilizeMemory(int requiredMemorySize) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(requiredMemorySize < 0,
		"initilizeMemory: requested memory size less than 0");

	exceptionGenerator.generateWarning(requiredMemorySize == 0,
		"initilizeMemory: requested memory size is 0");

	memoryPoolPointer = (MemoryType*)malloc(requiredMemorySize * sizeof(MemoryType));

	exceptionGenerator.generateError(memoryPoolPointer == NULL,
		"initilizeMemory: cannot allocate memory");

	memoryLeft += requiredMemorySize;
	return 0;
};


template<typename MemoryType>
MemoryType* PseudoDynamicMemoryController<typename MemoryType>::
holdMemory(int quantity) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(quantity < 0,
		"holdMemory: requested memory size is less than 0");
	exceptionGenerator.generateError(quantity > memoryLeft,
		"holdMemory: out of memory");

	exceptionGenerator.generateWarning(quantity == 0,
		"holdMemory: requested memory size is 0");

	int oldShift = shift;
	shift += quantity;
	memoryLeft -= quantity;
	return memoryPoolPointer + oldShift;
};


template<typename MemoryType>
MemoryType* PseudoDynamicMemoryController<typename MemoryType>::
holdMemory(int quantity, MemoryType initialValue) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(quantity < 0,
		"holdMemory: requested memory size is less than 0");
	exceptionGenerator.generateError(quantity > memoryLeft,
		"holdMemory: out of memory");

	exceptionGenerator.generateWarning(quantity == 0,
		"holdMemory: requested memory size is 0");

	int oldShift = shift;
	shift += quantity;
	memoryLeft -= quantity;
	for (int i = 0; i < quantity; i++) {
		memoryPoolPointer[oldShift + i] = initialValue;
	}
	return memoryPoolPointer + oldShift;
};


template<typename MemoryType>
MemoryType* PseudoDynamicMemoryController<typename MemoryType>::
loadFile(char* readingFileName, int columnsTotal, int stringsTotal) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(columnsTotal < 0,
		"loadFile: number of columns less than 0");
	exceptionGenerator.generateError(stringsTotal < 0,
		"loadFile: number of strings less than 0");

	exceptionGenerator.generateWarning(columnsTotal == 0,
		"loadFile: number of columns equal to 0");
	exceptionGenerator.generateWarning(stringsTotal == 0,
		"loadFile: number of strings equal to 0");

	FILE* readingFile;
	errno_t err = fopen_s(&readingFile, readingFileName, "r");
	MemoryType* memoryStartPointer = holdMemory(columnsTotal * stringsTotal);

	exceptionGenerator.generateError(memoryStartPointer == NULL,
		"loadFile: cannot allocate memory");

	for (int i = 0; i < stringsTotal; i++) {
		for (int j = 0; j < columnsTotal; j++) {
			fscanf_s(readingFile, STRING_FORMAT, 
				(memoryStartPointer + columnsTotal * i + j));
		}
	}
	fclose(readingFile);
	return memoryStartPointer;
}


template<typename MemoryType>
int PseudoDynamicMemoryController<typename MemoryType>::
loadInfo(char* readingFileName, int* dimension, int* lowBandWidth) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(dimension < 0,
		"loadInfo: number of columns less than 0");
	exceptionGenerator.generateError(lowBandWidth < 0,
		"loadInfo: number of strings less than 0");

	exceptionGenerator.generateWarning(dimension == 0,
		"loadInfo: number of columns equal to 0");
	exceptionGenerator.generateWarning(lowBandWidth == 0,
		"loadInfo: number of strings equal to 0");

	FILE* readingFile;
	errno_t err = fopen_s(&readingFile, readingFileName, "r");
	fscanf_s(readingFile, "%d%d", dimension, lowBandWidth);

	fclose(readingFile);
	return 0;
}


template<typename MemoryType>
int PseudoDynamicMemoryController<typename MemoryType>::
printMemoryContent(int startElementNumber, int endElementNumber, int stringLength) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(startElementNumber < 0,
		"printMemoryContent: number of start element less than 0");
	exceptionGenerator.generateError(endElementNumber < 0,
		"printMemoryContent: number of end element less than 0");
	exceptionGenerator.generateError(stringLength < 0,
		"printMemoryContent: string size less than 0");
	exceptionGenerator.generateError(stringLength == 0,
		"printMemoryContent: string length equal to 0");

	exceptionGenerator.generateWarning(endElementNumber < startElementNumber,
		"printMemoryContent: number of end element less than number of start");

	int readingLength = endElementNumber - startElementNumber;
	int stirngsTotal = 0;
	if (stringLength != 0) {
		stirngsTotal = readingLength / stringLength;
	}
	for (int i = 0; i < stirngsTotal; i++) {
		for (int j = 0; j < stringLength; j++) {
			cout << *(memoryPoolPointer + i * stringLength + j + startElementNumber)
				<< " ";
		}
		cout << endl;
	}
	return 0;
}


template<typename MemoryType>
int PseudoDynamicMemoryController<typename MemoryType>::
printMemoryContent(MemoryType* pointer, int quantity, int stringLength) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(pointer == NULL,
		"printMemoryContent: pointer is NULL");
	exceptionGenerator.generateError(stringLength == 0,
		"printMemoryContent: string length equal to 0");

	exceptionGenerator.generateWarning(quantity == 0,
		"printMemoryContent: quantity equal to 0");

	for (int i = 0; i < (quantity / stringLength); i++) {
		for (int j = 0; j < stringLength; j++) {
			cout << *pointer << " ";
			pointer++;
		}
		cout << endl;
	}
	return 0;
}


template<typename MemoryType>
int PseudoDynamicMemoryController<typename MemoryType>::
printMemoryContent(MemoryType* pointer, int quantity, int stringLength, char* header) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(pointer == NULL,
		"printMemoryContent: pointer is NULL");
	exceptionGenerator.generateError(stringLength == 0,
		"printMemoryContent: string length equal to 0");
	exceptionGenerator.generateError(header == NULL,
		"printMemoryContent: header is NULL");

	exceptionGenerator.generateWarning(quantity == 0,
		"printMemoryContent: quantity equal to 0");

	cout << "--- " << header << " ---" << endl;
	for (int i = 0; i < (quantity / stringLength); i++) {
		for (int j = 0; j < stringLength; j++) {
			cout << *pointer << " ";
			pointer++;
		}
		cout << endl;
	}
	cout << endl;
	return 0;
}


template<typename MemoryType>
int PseudoDynamicMemoryController<typename MemoryType>::
printMemoryContentFormat(MemoryType* pointer, int quantity, int stringLength) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(pointer == NULL,
		"saveInFile: pointer is NULL");
	exceptionGenerator.generateError(stringLength == 0,
		"printMemoryContent: string length equal to 0");

	exceptionGenerator.generateWarning(quantity == 0,
		"printMemoryContent: quantity equal to 0");
	exceptionGenerator.generateWarning(stringLength == 0,
		"printMemoryContent: string equal to 0");

	for (int i = 0; i < (quantity / stringLength); i++) {
		for (int j = 0; j < stringLength; j++) {
			printf(STRING_OUTPUT_FORMAT, *(pointer + stringLength * i + j));
		}
	}

	return 0;
}


template<typename MemoryType>
void PseudoDynamicMemoryController<typename MemoryType>::printMemoryUsage() {
	if (shift == 0 && memoryLeft == 0) {
		cout << "memory pool is empty" << endl;
		return;
	}

	int memoryUsagePercent = 0;
	memoryUsagePercent = (int)((shift / ((shift + memoryLeft) / 100.0)));
	cout << "Used memory: " << memoryUsagePercent << "%" << endl;
}


template<typename MemoryType>
void PseudoDynamicMemoryController<typename MemoryType>::
saveInFile(char* writingFileName, MemoryType* pointer, int quantity, int stringLength) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(pointer == NULL,
		"saveInFile: pointer is NULL");
	exceptionGenerator.generateError(stringLength == 0,
		"printMemoryContent: string length equal to 0");

	exceptionGenerator.generateWarning(quantity == 0,
		"printMemoryContent: quantity equal to 0");
	exceptionGenerator.generateWarning(stringLength == 0,
		"printMemoryContent: string equal to 0");

	FILE* writingFile;
	errno_t err = fopen_s(&writingFile, writingFileName, "w");
	for (int i = 0; i < (quantity / stringLength); i++) {
		for (int j = 0; j < stringLength; j++) {
			fprintf_s(writingFile,
				STRING_OUTPUT_FORMAT, *(pointer + stringLength * i + j));
		}
	}

	fclose(writingFile);
	return;
}


template<typename MemoryType>
void PseudoDynamicMemoryController<typename MemoryType>::setAlertLevel(int newAlertLevel) {
	exceptionGenerator.setExceptionAlertLevel(newAlertLevel);
};


template<typename MemoryType>
void PseudoDynamicMemoryController<typename MemoryType>::finalize() {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(memoryLeft != 0,
		"finalize: not all memory was used");
};