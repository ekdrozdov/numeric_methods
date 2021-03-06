/**
* slae.cpp
* Purpose: solve the system of the linear algebraic equations
* that looks like Ax = b.
*
* @author Edward Drozdov
* @version 1.0 18/10/17
*/

#include "slae.h"


SLAESolver::SLAESolver() {
	dimension = 0;
	Au = NULL;
	Ad = NULL;
	Al = NULL;
	x = NULL;
	b = NULL;
	memoryController = new PseudoDynamicMemoryController<DATA_TYPE>;
};


SLAESolver::~SLAESolver() {
	dimension = 0;
	if (memoryController != NULL) {
		memoryController->~PseudoDynamicMemoryController();
		memoryController = NULL;
	}
	Au = NULL;
	Ad = NULL;
	Al = NULL;
	x = NULL;
	b = NULL;
};


void SLAESolver::saveResult(char* resultName) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(Al == NULL,
		"saveResult: low matrix pointer is NULL");

	memoryController->saveInFile(resultName, x, dimension, dimension);
};


void SLAESolver::setAlertLevel(int newAlertLevel) {
	exceptionGenerator.setExceptionAlertLevel(newAlertLevel);
	memoryController->setAlertLevel(newAlertLevel);
};


/*
* Section II
*	SLAESolverLDLT class implementation.
*
*/


SLAESolverLDLT::SLAESolverLDLT() {
	lowBandWidth = 0;
	isDecompose = 0;
};


SLAESolverLDLT::~SLAESolverLDLT() {
	lowBandWidth = 0;
	isDecompose = 0;
};


void SLAESolverLDLT::computeLDLTDecomposition() {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(Al == NULL,
		"computeLDLTDecomposition: low matrix pointer is NULL");
	exceptionGenerator.generateError(Ad == NULL,
		"computeLDLTDecomposition: matrix diagonal pointer is NULL");

	DATA_TYPE* _Al = Al;
	DATA_TYPE* _Ad = Ad;
	int n = dimension;
	int p = lowBandWidth;

	int im = 0;
	for (_Ad = &Ad[0]; _Ad < &Ad[p + 1]; _Ad++, im++) {
		SCALE sumD = 0;
		int jm = 0;
		int shiftL = im * p + p - im;
		for (DATA_TYPE* AdInn = Ad, *_Al = &Al[shiftL];
			AdInn < _Ad; AdInn++, _Al++, jm++) {

			SCALE sumL = 0;
			for (DATA_TYPE* Adk = Ad, *Ali = &Al[shiftL],
				*Alj = &Al[jm * p + p - jm];
				Ali < _Al; Adk++, Ali++, Alj++) {

				sumL += (SCALE)(*Adk * *Ali * *Alj);
			}
			*_Al = (*_Al - sumL) / *AdInn;
			sumD += (SCALE)(*AdInn * *_Al * *_Al);
		}
		*_Ad = *_Ad - sumD;
	}

	for (_Ad = &Ad[p + 1]; _Ad < &Ad[n]; _Ad++, im++) {
		SCALE sumD = 0;
		int jm = im - p;
		int shiftL = im * p;
		int shiftD = im - p;
		int jb = 0;
		for (DATA_TYPE* AdInn = &Ad[shiftD], *_Al = &Al[shiftL];
			AdInn < _Ad; AdInn++, _Al++, jm++, jb++) {

			SCALE sumL = 0;
			for (DATA_TYPE* Adk = &Ad[shiftD], *Ali = &Al[shiftL],
				*Alj = &Al[jm * p + p - jb];
				Ali < _Al; Adk++, Ali++, Alj++) {

				sumL += (SCALE)(*Adk * *Ali * *Alj);
			}
			*_Al = (*_Al - sumL) / *AdInn;
			sumD += (SCALE)(*AdInn * *_Al * *_Al);
		}
		*_Ad = *_Ad - sumD;
	}
	isDecompose = 1;
}


void SLAESolverLDLT::directRun() {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(Al == NULL,
		"directRun: low matrix pointer is NULL");
	exceptionGenerator.generateError(b == NULL,
		"directRun: solve vector pointer is NULL");
	exceptionGenerator.generateError(x == NULL,
		"directRun: variable vector pointer is NULL");

	int p = lowBandWidth;
	int n = dimension;

	DATA_TYPE* z = x;
	z[0] = b[0];
	z++;
	DATA_TYPE* pb = b + 1;
	for (int im = 1; im < p + 1; im++, z++, pb++) {
		SCALE sumZ = 0;
		for (DATA_TYPE* zInn = x, *li = &Al[im * p + p - im]; zInn < z; zInn++, li++) {
			sumZ += (SCALE)(*li * *zInn);
		}
		*z = *pb - sumZ;
	}

	for (int im = p + 1; im < n; im++, z++, pb++) {
		SCALE sumZ = 0;
		for (DATA_TYPE* zInn = &x[im - p], *li = &Al[im * p]; zInn < z; zInn++, li++) {
			sumZ += (SCALE)(*li * *zInn);
		}
		*z = *pb - sumZ;
	}
}


void SLAESolverLDLT::reverseRun() {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(Al == NULL,
		"reverseRun: low matrix pointer is NULL");
	exceptionGenerator.generateError(Ad == NULL,
		"reverseRun: matrix diagonal pointer is NULL");
	exceptionGenerator.generateError(x == NULL,
		"reverseRun: variable vector pointer is NULL");

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
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(dimension < 0,
		"copmuteRequiredMemorySize: dimension less than 0");
	exceptionGenerator.generateError(lowBandWidth < 0,
		"copmuteRequiredMemorySize: low band width less than 0");

	int memoryForMatrixL = dimension * lowBandWidth;
	int memoryForDiagonal = dimension;
	int memoryForVectorX = dimension;
	int memoryForVectorB = dimension;
	return memoryForMatrixL + memoryForDiagonal + memoryForVectorX + memoryForVectorB;
}


void SLAESolverLDLT::
loadMatrix(char* infoName, char* diName, char* aalName) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(infoName == NULL,
		"solve: info file name is NULL");
	exceptionGenerator.generateError(diName == NULL,
		"solve: di file name is NULL");
	exceptionGenerator.generateError(aalName == NULL,
		"solve: aal file name is NULL");

	isDecompose = 0;
	memoryController->loadInfo(infoName, &dimension, &lowBandWidth);
	memoryController->initilizeMemory(copmuteRequiredMemorySize());
	Al = memoryController->loadFile(aalName, lowBandWidth, dimension);
	Ad = memoryController->loadFile(diName, dimension, 1);
};


void SLAESolverLDLT::
solve(char* bName) {
	exceptionGenerator.checkIsError();
	exceptionGenerator.generateError(bName == NULL,
		"solve: b file name is NULL");
	exceptionGenerator.generateError(isDecompose == 0,
		"solve: matrix isn't decompose");

	b = memoryController->loadFile(bName, dimension, 1);
	x = memoryController->holdMemory(dimension, 0);
	memoryController->finalize();
	directRun();
	reverseRun();
};


void SLAESolverLDLT::printSLAE() {
	memoryController->printMemoryContent(Al, lowBandWidth * dimension, lowBandWidth, "l");
	memoryController->printMemoryContent(Ad, dimension, dimension, "d");
	memoryController->printMemoryContent(b, dimension, dimension, "b");
	memoryController->printMemoryContent(x, dimension, dimension, "x");
};


void SLAESolverLDLT::printSolveVector() {
	memoryController->printMemoryContentFormat(x, dimension, dimension);
};