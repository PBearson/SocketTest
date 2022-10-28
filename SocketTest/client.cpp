#include <iostream>
#include <WS2tcpip.h>
#include "headers.h"

#pragma comment (lib, "ws2_32.lib")

int run_client()
{
	// Initialize winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int winsock = WSAStartup(version, &data);

	if (winsock != 0)
	{
		return fail("WSAStartup", winsock);
	}

	// Cleanup (close) winsock
	WSACleanup();

	return 0;
}