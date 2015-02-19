#include <stdio.h>
#include "./CTestCase.h"

int CTestCase::count = 0;

CTestCase::CTestCase() {
	count++;
}

void CTestCase::Start() {
	printf("test %d start *************************************************\n",count);
}

void CTestCase::Stop() {
	printf("*************************************************** test end\n\n");
}
