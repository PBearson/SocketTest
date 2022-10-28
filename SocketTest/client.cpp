#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include "headers.h"

int run_client()
{
	std::string ip_addr = "127.0.0.1";
	int port = 50000;

	// Initialize winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int winsock = WSAStartup(version, &data);

	if (winsock != 0)
	{
		return fail("WSAStartup", winsock);
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		return fail("socket", WSAGetLastError());
	}

	// Fill in hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	if (-1 == inet_pton(AF_INET, ip_addr.c_str(), &hint.sin_addr))
	{
		return fail("inet_pton", WSAGetLastError());
	}

	// Connect to server
	int conn_result = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (conn_result == SOCKET_ERROR)
	{
		return fail("connect", WSAGetLastError());
	}
	printf("Connected to %s:%d\n", ip_addr.c_str(), port);

	// While loop to send/receive data
	char buf[4096];
	std::string input;

	do
	{
		printf("> ");
		getline(std::cin, input);

		if (input.size() == 0)
		{
			printf("Client disconnected\n");
			break;
		}

		// Send input
		int bytes_sent = send(sock, input.c_str(), input.size() + 1, 0);
		if (bytes_sent == SOCKET_ERROR)
		{
			return fail("send", WSAGetLastError());
		}

		// Wait for response
		memset(buf, 0, 4096);
		int bytes_received = recv(sock, buf, 4096, 0);
		if (bytes_received == SOCKET_ERROR)
		{
			return fail("recv", WSAGetLastError());
		}

		// Echo message back to console
		if (bytes_received > 0)
		{
			printf("SERVER> %s\n", buf);
		}
	} while (input.size() > 0);

	// Cleanup everything
	closesocket(sock);
	WSACleanup();

	return 0;
}