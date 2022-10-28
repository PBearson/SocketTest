#include <iostream>
#include <string>
#include "headers.h"

#pragma comment (lib, "ws2_32.lib")

int main(int argc, char** argv)
{
	std::string select;
	printf("Run server (s) or client (c)? ");
	getline(std::cin, select);
	//if (/*0 == scanf_s("%c", &select, 1))
	//{
		//return fail("scanf", 0);
	//}

	if (select == "c") return run_client();
	else if (select == "s") return run_server();
	else
	{
		printf("I don't recognize argument %c\n", select);
		return 0;
	}
}