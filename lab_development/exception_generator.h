/**
* exception_generator.h
* Purpose: generate exceptions
*
* @author Edward Drozdov
* @version 1.0d 19/10/17
* inspired by Zadorojny A.G. and Pleshkova A.A.
*/

#pragma once
#include <iostream>

#define ERROR_INFO_MAX_LENGTH 70
#define WARNING_INFO_MAX_LENGTH 70


class ExceptionGenerator {
public:
	ExceptionGenerator();

	inline int getErrorsCount() { return errorsCount; };
	inline int getWarningsCount() { return warningsCount; };

	void checkIsError();
	void generateError(bool errorCondition, char* errorInfo);
	void generateWarning(bool warningCondition, char* warningInfo);
	void printErrorInfo();
	void printWarningInfo();
	void setExceptionAlertLevel(int newAlertLevel);

private:
	int errorsCount;
	int warningsCount;
	int exceptionAlertLevel;
	char* errorInfo[ERROR_INFO_MAX_LENGTH];
	char* warningInfo[WARNING_INFO_MAX_LENGTH];
};