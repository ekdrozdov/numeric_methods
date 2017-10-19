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

	inline DATA_TYPE* getAl() { return Al; };
	inline DATA_TYPE* getAd() { return Ad; };
	inline DATA_TYPE* getB() { return b; };
	inline DATA_TYPE* getX() { return x; };

	inline int getDimension() { return dimension; };

	virtual void printSLAE() = 0;

protected:
	int dimension;
	PseudoDynamicMemoryController* memoryController;
	DATA_TYPE* Au;
	DATA_TYPE* Ad; 
	DATA_TYPE* Al; 
	DATA_TYPE* x; 
	DATA_TYPE* b;
	
	int errorsCount;
	int warningsCount;
	char* errorInfo[ERROR_INFO_STRING_LENGTH];
	char* warningInfo[WARNING_INFO_STRING_LENGTH];
};


class SLAESolverLDLT: public SLAESolver {
public:
	void solve(char* infoName, char* diName, char* aalName, char* bName);
	void printSLAE();

	int copmuteRequiredMemorySize(int dimension, int lowBandWidth);
	void computeLDLTDecomposition(DATA_TYPE* pALow, DATA_TYPE* pADiagonal,
		int lowBandWidth, int matrixDimension);
	void reverseRun(DATA_TYPE* l, DATA_TYPE* d, DATA_TYPE* x, DATA_TYPE* b,
		int dimension, int lowBandWidth);
	void directRun(DATA_TYPE* l, DATA_TYPE* x, DATA_TYPE* b,
		int dimension, int lowBandWidth);

	inline int getLowBandWidth() { return lowBandWidth; };

private:
	int lowBandWidth;
};