/**
* tester.h
* Purpose: automate testing of the developing sovers:
* Files with test cases must be named as "case_#",
* where # -- is a number, starts from 1.
*
* Issues:
*	How to define a result type, if it is a vector with unknown length?
*	How to include a solver in tester?
*	How to pass arguments in solver, if it is a unknown amount of files?
*
* @author Edward Drozdov
* @version 1.0d 17/10/17
*/

#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "tester.h"

#define TEST_FILE_PREFIX "case_"
#define TEST_FILE_EXTENSION ".txt"
#define RESULT_FILE_PREFIX "result_"
#define RESULT_FILE_EXTENSION ".txt"
#define RESULT_TYPE int // Todo: define result type properly.


using namespace std;

string get_testfile_name(int testFileNumber) {
	return TEST_FILE_PREFIX + to_string(testFileNumber) + TEST_FILE_EXTENSION;
}


string get_resultfile_name(int resultFileNumber) {
	return RESULT_FILE_PREFIX + to_string(resultFileNumber) + RESULT_FILE_EXTENSION;
}


RESULT_TYPE get_result(string resultFileName) { // fix this function
	fstream resultFile;
	resultFile.open(resultFileName);
	int result;
	resultFile >> result;
	resultFile.close();
	return 2;
}


RESULT_TYPE solve(string testtFileName) { // fix this
	return 1;
}


int estimate_error(RESULT_TYPE expectedResult, RESULT_TYPE actualResult) {
	return abs(actualResult - expectedResult) / actualResult * 100;
}


int run_all_tests(int testsTotal, string testReportFileName) {
	if (testsTotal <= 0) {
		cout << "build_test_set failed: number of tests is incorrect";
		return -1;
	}

	fstream testReportFile;

	testReportFile.open(testReportFileName);
	for (int testNumber = 0; testNumber < testsTotal; testNumber++) {
		string testFileName = get_testfile_name(testNumber);
		string resultFileName = get_resultfile_name(testNumber);

		RESULT_TYPE expectedResult = get_result(resultFileName);
		RESULT_TYPE actualResult = solve(testFileName);

		int error = estimate_error(expectedResult, actualResult);
		testReportFile << "error in " << testFileName << ": " << error << "%\n";
	}
	testReportFile.close();

	return 0;
}


