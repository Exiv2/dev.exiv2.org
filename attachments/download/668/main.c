#include <stdio.h>
#include <windows.h>

int main(int argc, char** argv)
{
	printf("Opening file '%s' with FILE_SHARE_DELETE acess...\n", argv[1]);

	HANDLE hFile = CreateFile(argv[1], GENERIC_READ,
			FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,	NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		printf("failed to open file\n");
		return -1;
	}

	sleep(10);

	CloseHandle(hFile);

	return 0;
}

