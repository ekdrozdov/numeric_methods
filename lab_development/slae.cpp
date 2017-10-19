/**
* slae.cpp
* Purpose: solve the system of the linear algebraic equations
* that looks like Ax = b.
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

	errorsCount = 0;
	warningsCount = 0;
	*errorInfo = ERROR_INFO_SUCCESSFUL;
	*warningInfo = WARNING_INFO_SUCCESSFUL;
};


void SLAESolverLDLT::computeLDLTDecomposition(DATA_TYPE* pALow, DATA_TYPE* pADiagonal,
	int lowBandWidth, int matrixDimension) {
	if (errorsCount > 0) {
		return;
	}
	else if (pALow == 0) {
		errorsCount++;
		*errorInfo = "initilizeMemory: requested memory size less than 0";
		return;
	}

	DATA_TYPE* Al = pALow;
	DATA_TYPE* Ad = pADiagonal;
	int n = matrixDimension;
	int p = lowBandWidth;

	int im = 0;
	for (Ad = &pADiagonal[0]; Ad < &pADiagonal[p + 1]; Ad++, im++) {
		DATA_TYPE sumD = 0;
		int jm = 0;
		int shiftL = im * p + p - im;
		for (DATA_TYPE* AdInn = pADiagonal, *Al = &pALow[shiftL];
			AdInn < Ad; AdInn++, Al++, jm++) {

			DATA_TYPE sumL = 0;
			for (DATA_TYPE* Adk = pADiagonal, *Ali = &pALow[shiftL],
				*Alj = &pALow[jm * p + p - jm];
				Ali < Al; Adk++, Ali++, Alj++) {

				sumL += *Adk * *Ali * *Alj;
			}
			*Al = (*Al - sumL) / *AdInn;
			sumD += *AdInn * *Al * *Al;
		}
		*Ad = *Ad - sumD;
	}

	for (Ad = &pADiagonal[p + 1]; Ad < &pADiagonal[n]; Ad++, im++) {
		DATA_TYPE sumD = 0;
		int jm = im - p;
		int shiftL = im * p;
		int shiftD = im - p;
		int jb = 0;
		for (DATA_TYPE* AdInn = &pADiagonal[shiftD], *Al = &pALow[shiftL];
			AdInn < Ad; AdInn++, Al++, jm++, jb++) {

			DATA_TYPE sumL = 0;
			for (DATA_TYPE* Adk = &pADiagonal[shiftD], *Ali = &pALow[shiftL],
				*Alj = &pALow[jm * p + p - jb];
				Ali < Al; Adk++, Ali++, Alj++) {

				sumL += *Adk * *Ali * *Alj;
			}
			*Al = (*Al - sumL) / *AdInn;
			sumD += *AdInn * *Al * *Al;
		}
		*Ad = *Ad - sumD;
	}
}


void SLAESolverLDLT::directRun(DATA_TYPE* l, DATA_TYPE* x, DATA_TYPE* b,
	int dimension, int lowBandWidth) {

	int p = lowBandWidth;
	int n = dimension;

	DATA_TYPE* z = x;
	z[0] = b[0];
	z++;
	DATA_TYPE* pb = b + 1;
	for (int im = 1; im < p + 1; im++, z++, pb++) {
		DATA_TYPE sumZ = 0;
		for (DATA_TYPE* zInn = x, *li = &l[im * p + p - im]; zInn < z; zInn++, li++) {
			sumZ += *li * *zInn;
		}
		*z = *pb - sumZ;
	}

	for (int im = p + 1; im < n; im++, z++, pb++) {
		DATA_TYPE sumZ = 0;
		for (DATA_TYPE* zInn = &x[im - p], *li = &l[im * p]; zInn < z; zInn++, li++) {
			sumZ += *li * *zInn;
		}
		*z = *pb - sumZ;
	}
}


void SLAESolverLDLT::reverseRun(DATA_TYPE* l, DATA_TYPE* d, DATA_TYPE* x, DATA_TYPE* b,
	int dimension, int lowBandWidth) {

	int p = lowBandWidth;
	int n = dimension;

	DATA_TYPE* y = x;
	DATA_TYPE* pd = d;
	for (int im = 0; im < n; im++, y++, pd++) {
		*y = *y / *pd;
	}

	DATA_TYPE* px = &x[n - 1];
	pd = &d[n - 1];
	for (int im = n - 1; im > p; im--, px--, pd--) {
		for (DATA_TYPE* xInn = &x[im - p], *pl = &l[im * p];
			xInn < px; xInn++, pl++) {
			*xInn = *xInn - *pl * *px;
		}
	}

	for (int im = p; im >= 0; im--, px--, pd--) {
		for (DATA_TYPE* xInn = x, *pl = &l[im * p + p - im];
			xInn < px; xInn++, pl++) {
			*xInn = *xInn - *pl * *px;
		}
	}
}


int SLAESolverLDLT::copmuteRequiredMemorySize(int dimension, int lowBandWidth) {
	int memoryForMatrixL = dimension * lowBandWidth;
	int memoryForDiagonal = dimension;
	int memoryForVectorX = dimension;
	int memoryForVectorB = dimension;
	return memoryForMatrixL + memoryForDiagonal + memoryForVectorX + memoryForVectorB;
}


void SLAESolverLDLT::
solve(char* infoName, char* diName, char* aalName, char* bName) {
	dimension = 0;
	lowBandWidth = 0;
	memoryController = new PseudoDynamicMemoryController;
	memoryController->loadInfo(infoName, &dimension, &lowBandWidth);
	memoryController->initilizeMemory(copmuteRequiredMemorySize(dimension, lowBandWidth));

	Al = memoryController->loadFile(aalName, lowBandWidth, dimension);
	Ad = memoryController->loadFile(diName, dimension, 1);

	computeLDLTDecomposition(Al, Ad, lowBandWidth, dimension);

	b = memoryController->loadFile(bName, dimension, 1);
	x = memoryController->holdMemory(dimension, 0);
	directRun(Al, x, b, dimension, lowBandWidth);
	reverseRun(Al, Ad, x, b, dimension, lowBandWidth);
};


void SLAESolverLDLT::printSLAE() {
	memoryController->printMemoryContent(Al, lowBandWidth * dimension, lowBandWidth, "l");
	memoryController->printMemoryContent(Ad, dimension, dimension, "d");
	memoryController->printMemoryContent(b, dimension, dimension, "b");
	memoryController->printMemoryContent(x, dimension, dimension, "x");
};
