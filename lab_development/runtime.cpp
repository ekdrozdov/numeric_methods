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


using namespace std;

void testTester() {
	int testsTotal = 2;
	string testReportFileName = "report.txt";
	run_all_tests(testsTotal, testReportFileName);
}


void shutdown() {
	cout << "Press any key to continue...";
	getchar();
}

 
int main(int argc, char* argv[]) {
	SLAESolverLDLT slae;

	slae.solve("info.txt", "di.txt", "aal.txt", "b.txt");
	slae.printSLAE();
		
	shutdown();
	return EXIT_SUCCESS;
}