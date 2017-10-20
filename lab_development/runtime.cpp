/**
* runtipme.cpp
* Purpose: emulate extern program that use developing classes.
*
* @author Edward Drozdov
* @version 1.0d 17/10/17
*/

#pragma once
#include <stdio.h>
#include "tester.h"
#include "pseudo_dynamic_memory_controller.h"
#include "slae.h"
#include "arguments_parser.h"

#define MAX_FILE_NAME_LENGTH 20
#define DEFAULT_ALERT_LEVEL 1
#define MAX_ARGUMENTS_NUMBER 8
#define MIN_ARGUMENTS_NUMBER 3
#define DEFAULT_AAL_NAME "aal.txt"
#define DEFAULT_B_NAME "b.txt"
#define DEFAULT_DI_NAME "di.txt"
#define DEFAULT_INFO_NAME "info.txt"


using namespace std;

void testTester() {
	int testsTotal = 2;
	string testReportFileName = "report.txt";
	run_all_tests(testsTotal, testReportFileName);
}


void shutdown() {
	// Activate when debugging.
	//cout << "Press any key to continue...";
	//getchar();
}


void printUsage() {
	cout << "Usage:" << endl;
	cout << "slaesolver [-l (0, 1, 2)] [-p] [-ds] infoName aalName diName bName" << endl;
	cout << "-l means logging level" << endl;
	cout << "-p means enable output of the solve vector" << endl;
	cout << "-ds means disable saving result vector into file" << endl;
}


int main(int argc, char* argv[]) {
	if (argc < MIN_ARGUMENTS_NUMBER && argc != 1) {
		printf("Too few arguments supplied.\n");
		printUsage();
		shutdown();
		return EXIT_FAILURE;
	}
	else if (argc > MAX_ARGUMENTS_NUMBER + 1) {
		printf("Too many arguments supplied.\n");
		printUsage();
		shutdown();
		return EXIT_FAILURE;
	}
	
	char* diName[MAX_FILE_NAME_LENGTH];
	char* aalName[MAX_FILE_NAME_LENGTH];
	char* infoName[MAX_FILE_NAME_LENGTH];
	char* bName[MAX_FILE_NAME_LENGTH];

	int alertLevel = DEFAULT_ALERT_LEVEL;
	bool isOutputEnabled = 0;
	bool isSavingEnabled = 1;

	if (argc != 1) {
		if (*argv[argc - 1] == '-' || *argv[argc - 2] == '-' ||
			*argv[argc - 3] == '-' || *argv[argc - 4] == '-') {
			printf("Wrong arguments.\n");
			printUsage();
			shutdown();
			return EXIT_FAILURE;
		}

		int i = 0;		
		for (char* p = argv[argc - 2]; *p != '\0'; p++, i++) {
			diName[i] = p;
		}		
		i = 0;
		for (char* p = argv[argc - 3]; *p != '\0'; p++, i++) {
			aalName[i] = p;
		}		
		i = 0;
		for (char* p = argv[argc - 4]; *p != '\0'; p++, i++) {
			infoName[i] = p;
		}		
		i = 0;
		for (char* p = argv[argc - 1]; *p != '\0'; p++, i++) {
			bName[i] = p;
		}

		InputParser input(argc, argv);
		if (input.cmdOptionExists("-l")) {
			alertLevel = stoi(input.getCmdOption("-l"));
		}
		if (input.cmdOptionExists("-p")) {
			isOutputEnabled = 1;
		}
		if (input.cmdOptionExists("-ds")) {
			isSavingEnabled = 0;
		}
	}
	else {
		*aalName = DEFAULT_AAL_NAME;
		*bName = DEFAULT_B_NAME;
		*diName = DEFAULT_DI_NAME;
		*infoName = DEFAULT_INFO_NAME;
	}
	
	SLAESolverLDLT slae;

	slae.setAlertLevel(alertLevel);
	slae.loadMatrix(*infoName, *diName, *aalName);
	slae.computeLDLTDecomposition();
	slae.solve(*bName);
	if (isOutputEnabled == 1) {
		slae.printSolveVector();
	}
	//slae.printSLAE();
	if (isSavingEnabled == 1) {
		slae.saveResult("result.txt");
	}
	//slae.~SLAESolverLDLT();
	shutdown();
	return EXIT_SUCCESS;
}