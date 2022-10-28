#include "headers.h"
#include <iostream>
#include <WS2tcpip.h>

int fail(const char* rtn, int code)
{
	printf("%s failed with error %d\n", rtn, code);
	WSACleanup();
	return code;
}