#include <iostream>
#include "headers.h"

int main(int argc, char** argv)
{
	char select = 0;
	printf("Run server (s) or client (c)? ");
	if (0 == scanf_s("%c", &select, 1))
	{
		return fail("scanf", 0);
	}

	if (select == 'c') return run_client();
	else if (select == 's') return run_server();
	else
	{
		printf("I don't recognize argument %c\n", select);
		return 0;
	}
}