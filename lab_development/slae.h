/**
* slae.h
* Purpose: solve the system of the linear algebraic equations
* that looks like Ax = b.
*
* @author Edward Drozdov
* @version 1.0d 17/10/17
*/

#pragma once
#include <string>
#include <fstream>
#include "pseudo_dynamic_memory_controller.h"
#include <iostream>
#include <fstream>

#define DATA_TYPE double
#define ERROR_INFO_STRING_LENGTH 70
#define WARNING_INFO_STRING_LENGTH 70


using namespace std;

class SLAESolver {
public:	
	SLAESolver();

	virtual void solve(char* infoName, char* diName, char* aalName, char* bName) = 0;
	virtual void printSLAE() = 0;
	void saveResult(char* resultName);

	void setAlertLevel(int newAlertLevel);

	inline DATA_TYPE* getAl() { return Al; };
	inline DATA_TYPE* getAd() { return Ad; };
	inline DATA_TYPE* getB() { return b; };
	inline DATA_TYPE* getX() { return x; };
	inline int getDimension() { return dimension; };	

protected:
	int dimension;
	PseudoDynamicMemoryController* memoryController;
	ExceptionGenerator exceptionGenerator;
	DATA_TYPE* Au;
	DATA_TYPE* Ad; 
	DATA_TYPE* Al; 
	DATA_TYPE* x; 
	DATA_TYPE* b;
};


class SLAESolverLDLT: public SLAESolver {
public:
	SLAESolverLDLT();

	void solve(char* infoName, char* diName, char* aalName, char* bName);
	void printSLAE();	

	inline int getLowBandWidth() { return lowBandWidth; };

private:
	int copmuteRequiredMemorySize();
	void computeLDLTDecomposition();
	void reverseRun();
	void directRun();
	int lowBandWidth;
};