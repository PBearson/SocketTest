#include "headers.h"
#include <iostream>

int fail(const char* rtn, int code)
{
	printf("%s failed with error %d\n", rtn, code);
	return code;
}