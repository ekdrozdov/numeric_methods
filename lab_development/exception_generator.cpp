/**
* exception_generator.cpp
* Purpose: generate exceptions
*
* @author Edward Drozdov
* @version 1.0d 19/10/17
* inspired by Zadorojny A.G. and Pleshkova A.A.
*/

#include "exception_generator.h"
#include <stdio.h>

#define ERROR_INFO_SUCCESSFUL "There is no errors"
#define WARNING_INFO_SUCCESSFUL "There is no warnings"


using namespace std;

void debugShutdown() {
	// Activate when debugging.
	//cout << "Press any key to continue...";
	//getchar();
}


ExceptionGenerator::ExceptionGenerator() {
	errorsCount = 0;
	warningsCount = 0;
	exceptionAlertLevel = 1;
	*errorInfo = ERROR_INFO_SUCCESSFUL;
	*warningInfo = WARNING_INFO_SUCCESSFUL;
};


void ExceptionGenerator::checkIsError() {
	if (errorsCount > 0) {
		cout << "ERROR: unknown error" << endl;
		debugShutdown();
		exit(EXIT_FAILURE);
	}
};


void ExceptionGenerator::generateError(bool errorCondition, char* errorInfo) {
	if (errorCondition) {
		errorsCount++;
		*(this->errorInfo) = errorInfo;
		if (exceptionAlertLevel > 0) {
			printErrorInfo();
		}
		debugShutdown();
		exit(EXIT_FAILURE);
	}
};


void ExceptionGenerator::generateWarning(bool warningCondition, char* warningInfo) {
	if (warningCondition) {
		warningsCount++;
		*(this->warningInfo) = warningInfo;
		if (exceptionAlertLevel > 1) {
			printWarningInfo();
		}
	}
};


void ExceptionGenerator::printErrorInfo() {
	cout << "ERROR: " << *errorInfo << endl;
};


void ExceptionGenerator::printWarningInfo() {
	cout << "WARNING: " << *warningInfo << endl;
};


void ExceptionGenerator::setExceptionAlertLevel(int newAlertLevel) {
	if ((newAlertLevel > 2) || (newAlertLevel < 0)) {
		cout << "WARNING: setExceptionAlertLevel: incorrect alert level, set as 0" << endl;
		exceptionAlertLevel = 0;
	}
	exceptionAlertLevel = newAlertLevel;
};