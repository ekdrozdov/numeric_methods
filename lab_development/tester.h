/**
* tester.h
* Purpose: automate testing of the developing sovers:
* Files with test cases must be named as "case_#",
* where # -- is a number, starts from 1.
*
* @author Edward Drozdov
* @version 1.0d 17/10/17
*/

#pragma once
#include <fstream>
#include <string>
#include <iostream>


using namespace std;

int run_all_tests(int testsTotal, string testReportFileName);