/**
* slae.cpp
* Purpose: solve the system of the linear algebraic equations
* that looks like Ax = b.
*
* Today changes:
*	connected to github!
*	resolved names collision and removed extra args from func parameters
*	exception generation was added
*
* @author Edward Drozdov
* @version 1.0 18/10/17
*/
 
#include "slae.h"

#define ERROR_INFO_SUCCESSFUL "There is no errors"
#define WARNING_INFO_SUCCESSFUL "There is no warnings"


SLAESolver::SLAESolver() {
	Au = NULL;
	Ad = NULL;
	Al = NULL;
	x = NULL;
	b = NULL;
	memoryController = NULL;

	dimension = 0;
	
	errorsCount = 0;
	warningsCount = 0;
	*errorInfo = ERROR_INFO_SUCCESSFUL;
	*warningInfo = WARNING_INFO_SUCCESSFUL;
};



void SLAESolver::saveResult(char* reslutName) {
	if (errorsCount > 0) {
		return;
	}
};


/*
* SLAESolverLDLT class implementation.
*/

SLAESolverLDLT::SLAESolverLDLT() {
	Au = NULL;
	Ad = NULL;
	Al = NULL;
	x = NULL;
	b = NULL;
	memoryController = NULL;

	dimension = 0;
	lowBandWidth = 0;

	errorsCount = 0;
	warningsCount = 0;
	*errorInfo = ERROR_INFO_SUCCESSFUL;
	*warningInfo = WARNING_INFO_SUCCESSFUL;
};


void SLAESolverLDLT::computeLDLTDecomposition() {
	if (errorsCount > 0) {
		return;
	}
	else if (Al == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: low matrix pointer is NULL";
		return;
	}
	else if (Ad == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: matrix diagonal pointer is NULL";
		return;
	}

	DATA_TYPE* _Al = Al;
	DATA_TYPE* _Ad = Ad;
	int n = dimension;
	int p = lowBandWidth;

	int im = 0;
	for (_Ad = &Ad[0]; _Ad < &Ad[p + 1]; _Ad++, im++) {
		DATA_TYPE sumD = 0;
		int jm = 0;
		int shiftL = im * p + p - im;
		for (DATA_TYPE* AdInn = Ad, *_Al = &Al[shiftL];
			AdInn < _Ad; AdInn++, _Al++, jm++) {

			DATA_TYPE sumL = 0;
			for (DATA_TYPE* Adk = Ad, *Ali = &Al[shiftL],
				*Alj = &Al[jm * p + p - jm];
				Ali < _Al; Adk++, Ali++, Alj++) {

				sumL += *Adk * *Ali * *Alj;
			}
			*_Al = (*_Al - sumL) / *AdInn;
			sumD += *AdInn * *_Al * *_Al;
		}
		*_Ad = *_Ad - sumD;
	}

	for (_Ad = &Ad[p + 1]; _Ad < &Ad[n]; _Ad++, im++) {
		DATA_TYPE sumD = 0;
		int jm = im - p;
		int shiftL = im * p;
		int shiftD = im - p;
		int jb = 0;
		for (DATA_TYPE* AdInn = &Ad[shiftD], *_Al = &Al[shiftL];
			AdInn < _Ad; AdInn++, _Al++, jm++, jb++) {

			DATA_TYPE sumL = 0;
			for (DATA_TYPE* Adk = &Ad[shiftD], *Ali = &Al[shiftL],
				*Alj = &Al[jm * p + p - jb];
				Ali < _Al; Adk++, Ali++, Alj++) {

				sumL += *Adk * *Ali * *Alj;
			}
			*_Al = (*_Al - sumL) / *AdInn;
			sumD += *AdInn * *_Al * *_Al;
		}
		*_Ad = *_Ad - sumD;
	}
}


void SLAESolverLDLT::directRun() {
	if (errorsCount > 0) {
		return;
	}
	else if (Al == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: low matrix pointer is NULL";
		return;
	}
	else if (b == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: solve vector pointer is NULL";
		return;
	}
	else if (x == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: variable vector pointer is NULL";
		return;
	}

	int p = lowBandWidth;
	int n = dimension;

	DATA_TYPE* z = x;
	z[0] = b[0];
	z++;
	DATA_TYPE* pb = b + 1;
	for (int im = 1; im < p + 1; im++, z++, pb++) {
		DATA_TYPE sumZ = 0;
		for (DATA_TYPE* zInn = x, *li = &Al[im * p + p - im]; zInn < z; zInn++, li++) {
			sumZ += *li * *zInn;
		}
		*z = *pb - sumZ;
	}

	for (int im = p + 1; im < n; im++, z++, pb++) {
		DATA_TYPE sumZ = 0;
		for (DATA_TYPE* zInn = &x[im - p], *li = &Al[im * p]; zInn < z; zInn++, li++) {
			sumZ += *li * *zInn;
		}
		*z = *pb - sumZ;
	}
}


void SLAESolverLDLT::reverseRun() {
	if (errorsCount > 0) {
		return;
	}
	else if (Al == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: low matrix pointer is NULL";
		return;
	}
	else if (Ad == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: matrix diagonal pointer is NULL";
		return;
	}
	else if (x == NULL) {
		errorsCount++;
		*errorInfo = "computeLDLTDecomposition: variable vector pointer is NULL";
		return;
	}

	int p = lowBandWidth;
	int n = dimension;

	DATA_TYPE* y = x;
	DATA_TYPE* pd = Ad;
	for (int im = 0; im < n; im++, y++, pd++) {
		*y = *y / *pd;
	}

	DATA_TYPE* px = &x[n - 1];
	pd = &Ad[n - 1];
	for (int im = n - 1; im > p; im--, px--, pd--) {
		for (DATA_TYPE* xInn = &x[im - p], *pl = &Al[im * p];
			xInn < px; xInn++, pl++) {
			*xInn = *xInn - *pl * *px;
		}
	}

	for (int im = p; im >= 0; im--, px--, pd--) {
		for (DATA_TYPE* xInn = x, *pl = &Al[im * p + p - im];
			xInn < px; xInn++, pl++) {
			*xInn = *xInn - *pl * *px;
		}
	}
}


int SLAESolverLDLT::copmuteRequiredMemorySize() {
	if (errorsCount > 0) {
		return -1;
	}
	else if (dimension < 0) {
		errorsCount++;
		*errorInfo = "copmuteRequiredMemorySize: dimension less than 0";
		return -1;
	}
	else if (lowBandWidth < 0) {
		errorsCount++;
		*errorInfo = "copmuteRequiredMemorySize: low band width less than 0";
		return -1;
	}

	int memoryForMatrixL = dimension * lowBandWidth;
	int memoryForDiagonal = dimension;
	int memoryForVectorX = dimension;
	int memoryForVectorB = dimension;
	return memoryForMatrixL + memoryForDiagonal + memoryForVectorX + memoryForVectorB;
}


void SLAESolverLDLT::
solve(char* infoName, char* diName, char* aalName, char* bName) {
	if (errorsCount > 0) {
		return;
	}

	dimension = 0;
	lowBandWidth = 0;
	memoryController = new PseudoDynamicMemoryController;
	memoryController->loadInfo(infoName, &dimension, &lowBandWidth);

	memoryController->initilizeMemory(copmuteRequiredMemorySize());

	Al = memoryController->loadFile(aalName, lowBandWidth, dimension);
	Ad = memoryController->loadFile(diName, dimension, 1);

	computeLDLTDecomposition();

	b = memoryController->loadFile(bName, dimension, 1);
	x = memoryController->holdMemory(dimension, 0);
	directRun();
	reverseRun();
};


void SLAESolverLDLT::printSLAE() {
	memoryController->printMemoryContent(Al, lowBandWidth * dimension, lowBandWidth, "l");
	memoryController->printMemoryContent(Ad, dimension, dimension, "d");
	memoryController->printMemoryContent(b, dimension, dimension, "b");
	memoryController->printMemoryContent(x, dimension, dimension, "x");
};
