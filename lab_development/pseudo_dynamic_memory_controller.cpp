/**
* pseudo_dynamic_memory_controller.cpp
* Purpose: create and control a heap.
*
* @author Edward Drozdov
* @version 1.0d 18/10/17
*/

#include "pseudo_dynamic_memory_controller.h"

#define ERROR_INFO_SUCCESSFUL "There is no errors"
#define WARNING_INFO_SUCCESSFUL "There is no warnings"


using namespace std;

PseudoDynamicMemoryController::PseudoDynamicMemoryController() {
	shift = 0;
	memoryLeft = 0;

	errorsCount = 0;
	warningsCount = 0;
	*errorInfo = ERROR_INFO_SUCCESSFUL;
	*warningInfo = WARNING_INFO_SUCCESSFUL;
};


int PseudoDynamicMemoryController::initilizeMemory(int requiredMemorySize) {
	if (errorsCount > 0) {
		return -1;
	}
	else if (requiredMemorySize < 0) {
		errorsCount++;
		*errorInfo = "initilizeMemory: requested memory size less than 0";
		return -1;
	}

	if (requiredMemorySize == 0) {
		warningsCount++;
		*warningInfo = "initilizeMemory: requested memory size is 0";
	}

	memoryPoolPointer = (MEMORY_TYPE*)malloc(requiredMemorySize * sizeof(MEMORY_TYPE));

	if (memoryPoolPointer == NULL) {
		errorsCount++;
		*errorInfo = "initilizeMemory: cannot allocate memory";
		return -1;
	}

	memoryLeft += requiredMemorySize;
	return 0;
};


MEMORY_TYPE* PseudoDynamicMemoryController::holdMemory(int quantity) {
	if (errorsCount > 0) {
		return NULL;
	}
	else if (quantity < 0) {
		errorsCount++;
		*errorInfo = "holdMemory: requested memory size is less than 0";
		return NULL;
	}
	else if (quantity > memoryLeft) {
		errorsCount++;
		*errorInfo = "holdMemory: out of memory";
		return NULL;
	}

	if (quantity == 0) {
		warningsCount++;
		*warningInfo = "holdMemory: requested memory size is 0";
	}

	int oldShift = shift;
	shift += quantity;
	memoryLeft -= quantity;
	return memoryPoolPointer + oldShift;
};


MEMORY_TYPE* PseudoDynamicMemoryController::
holdMemory(int quantity, MEMORY_TYPE initialValue) {
	if (errorsCount > 0) {
		return NULL;
	}
	else if (quantity < 0) {
		errorsCount++;
		*errorInfo = "holdMemory: requested memory size is less than 0";
		return NULL;
	}
	else if (quantity > memoryLeft) {
		errorsCount++;
		*errorInfo = "holdMemory: out of memory";
		return NULL;
	}

	if (quantity == 0) {
		warningsCount++;
		*warningInfo = "holdMemory: requested memory size is 0";
	}

	int oldShift = shift;
	shift += quantity;
	memoryLeft -= quantity;
	for (int i = 0; i < quantity; i++) {
		memoryPoolPointer[oldShift + i] = initialValue;
	}
	return memoryPoolPointer + oldShift;
};


MEMORY_TYPE* PseudoDynamicMemoryController::
loadFile(char* readingFileName, int columnsTotal, int stringsTotal) {
	if (errorsCount > 0) {
		return NULL;
	}
	else if (columnsTotal < 0) {
		errorsCount++;
		*errorInfo = "loadFile: number of columns less than 0";
		return NULL;
	}
	else if (stringsTotal < 0) {
		errorsCount++;
		*errorInfo = "loadFile: number of strings less than 0";
		return NULL;
	}

	if (columnsTotal == 0) {
		warningsCount++;
		*warningInfo = "loadFile: number of columns equal to 0";
	}
	if (stringsTotal == 0) {
		warningsCount++;
		*warningInfo = "loadFile: number of strings equal to 0";
	}

	FILE* readingFile;
	errno_t err = fopen_s(&readingFile, readingFileName, "r");
	MEMORY_TYPE* memoryStartPointer = holdMemory(columnsTotal * stringsTotal);
	if (memoryStartPointer == NULL) {
		errorsCount++;
		*errorInfo = "loadFile: cannot allocate memory";
		return NULL;
	}

	for (int i = 0; i < stringsTotal; i++) {
		for (int j = 0; j < columnsTotal; j++) {
			fscanf_s(readingFile, STRING_FORMAT, (memoryStartPointer + columnsTotal * i + j));
		}
	}
	fclose(readingFile);
	return memoryStartPointer;
}


int PseudoDynamicMemoryController::
loadInfo(char* readingFileName,	int* dimension, int* lowBandWidth) {
	if (errorsCount > 0) {
		return -1;
	}
	else if (dimension < 0) {
		errorsCount++;
		*errorInfo = "loadInfo: number of columns less than 0";
		return NULL;
	}
	else if (lowBandWidth < 0) {
		errorsCount++;
		*errorInfo = "loadInfo: number of strings less than 0";
		return NULL;
	}

	if (dimension == 0) {
		warningsCount++;
		*warningInfo = "loadInfo: number of columns equal to 0";
	}
	if (lowBandWidth == 0) {
		warningsCount++;
		*warningInfo = "loadInfo: number of strings equal to 0";
	}

	FILE* readingFile;
	errno_t err = fopen_s(&readingFile, readingFileName, "r");
	fscanf_s(readingFile, "%d%d", dimension, lowBandWidth);
	
	fclose(readingFile);
	return 0;
}


int PseudoDynamicMemoryController::
printMemoryContent(int startElementNumber, int endElementNumber, int stringLength) {
	if (errorsCount > 0) {
		return -1;
	}
	else if (startElementNumber < 0) {
		errorsCount++;
		*errorInfo = "printMemoryContent: number of start element less than 0";
		return -1;
	}
	else if (endElementNumber < 0) {
		errorsCount++;
		*errorInfo = "printMemoryContent: number of end element less than 0";
		return -1;
	}
	else if (stringLength < 0) {
		errorsCount++;
		*errorInfo = "printMemoryContent: string size less than 0";
		return -1;
	}

	if (endElementNumber < startElementNumber) {
		warningsCount++;
		*warningInfo = "printMemoryContent: number of end element less than number of start";
	}
	if (stringLength == 0) {
		errorsCount++;
		*errorInfo = "printMemoryContent: string length equal to 0";
	}

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
}


int PseudoDynamicMemoryController::printMemoryContent(MEMORY_TYPE* pointer, 
	int quantity, int stringLength) {
	if (errorsCount > 0) {
		return -1;
	}
	else if (pointer == NULL) {
		errorsCount++;
		*errorInfo = "printMemoryContent failed: pointer is NULL";
		return -1;
	}

	if (quantity == 0) {
		warningsCount++;
		*warningInfo = "printMemoryContent: quantity equal to 0";
	}
	if (stringLength == 0) {
		errorsCount++;
		*errorInfo = "printMemoryContent: string length equal to 0";
	}

	for (int i = 0; i < (quantity / stringLength); i++) {
		for (int j = 0; j < stringLength; j++) {
			cout << *pointer << " ";
			pointer++;
		}
		cout << endl;
	}
	return 0;
}


int PseudoDynamicMemoryController::
printMemoryContent(MEMORY_TYPE* pointer, int quantity, int stringLength, char* header) {
	if (errorsCount > 0) {
		return -1;
	}
	else if (pointer == NULL) {
		errorsCount++;
		*errorInfo = "printMemoryContent failed: pointer is NULL";
		return -1;
	}

	if (quantity == 0) {
		warningsCount++;
		*warningInfo = "printMemoryContent: quantity equal to 0";
	}
	if (stringLength == 0) {
		errorsCount++;
		*errorInfo = "printMemoryContent: string length equal to 0";
	}
	
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


void PseudoDynamicMemoryController::printErrorInfo() {
	cout << "ERROR: " << *errorInfo << endl;
}


void PseudoDynamicMemoryController::printWarningInfo() {
	cout << "WARNING: " << *warningInfo << endl;
}


void PseudoDynamicMemoryController::printMemoryUsage() {
	if (shift == 0 && memoryLeft == 0) {
		cout << "memory pool is empty" << endl;
		return;
	}
	int memoryUsagePercent = 0;
	memoryUsagePercent = (int)((shift / ((shift + memoryLeft) / 100.0)));
	cout << "Used memory: " << memoryUsagePercent << "%" << endl;
}