#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

int fail(const char* rtn, int code)
{
	printf("%s failed with error %d\n", rtn, code);
	return code;
}

int main()
{
	// Initialize winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int winsock = WSAStartup(version, &data);

	if (winsock != 0)
	{
		return fail("WSAStartup", winsock);
	}

	// Create a socket
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		return fail("socket", WSAGetLastError());
	}

	// Bind socket
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(50000);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	
	if (SOCKET_ERROR == bind(listen_sock, (const sockaddr*)&server_addr, sizeof(server_addr)))
	{
		return fail("bind", WSAGetLastError());
	}

	// Tell winsock the socket is for listening
	if (SOCKET_ERROR == listen(listen_sock, SOMAXCONN))
	{
		return fail("listen", WSAGetLastError());
	}
	char server_addr_s[16];
	inet_ntop(AF_INET, &server_addr.sin_addr.S_un.S_addr, server_addr_s, 16);
	printf("listening on %s:%d\n", server_addr_s, ntohs(server_addr.sin_port));

	// Wait for connection
	sockaddr_in client_addr;
	int client_size = sizeof(client_addr);

	SOCKET client_sock = accept(listen_sock, (sockaddr*)&client_addr, &client_size);
	if (client_sock == INVALID_SOCKET)
	{
		return fail("accept", WSAGetLastError());
	}

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	if (0 == getnameinfo((sockaddr*)&client_sock, sizeof(client_sock), host, NI_MAXHOST, service, NI_MAXSERV, 0))
	{
		printf("Client %s:%s connected\n", host, service);
	}
	else
	{
		char client_addr_s[16];
		inet_ntop(AF_INET, &client_addr.sin_addr.S_un.S_addr, client_addr_s, 16);
		u_short client_port = ntohs(client_addr.sin_port);
		printf("Client %s:%d connected\n", client_addr_s, client_port);
	}

	// Close listening socket
	closesocket(listen_sock);

	// Send welcome message to client
	char buf[4096];
	memset(buf, 0, 4096);
	sprintf_s(buf, "Hello from %s:%d\n", server_addr_s, ntohs(server_addr.sin_port));

	int bytes_sent = send(client_sock, buf, 32, 0);
	if (bytes_sent == SOCKET_ERROR)
	{
		return fail("send", WSAGetLastError());
	}
	printf("Echoed %s\n", buf);

	// while loop: accept and echo message back to client
	while (true)
	{
		memset(buf, 0, 4096);

		// Wait for client to send data
		int bytes_received = recv(client_sock, buf, 4096, 0);
		if (bytes_received == SOCKET_ERROR)
		{
			return fail("recv", WSAGetLastError());
		}

		if (bytes_received == 0)
		{
			printf("Client disconnected\n");
			break;
		}
		printf("Received %s\n", buf);

		// Echo message back to client
		int bytes_sent = send(client_sock, buf, bytes_received + 1, 0);
		if (bytes_sent == SOCKET_ERROR)
		{
			return fail("send", WSAGetLastError());
		}
		printf("Echoed %s\n", buf);
	}

	// Close socket
	closesocket(client_sock);

	// Cleanup (close) winsock
	WSACleanup();

	
	return 0;
}