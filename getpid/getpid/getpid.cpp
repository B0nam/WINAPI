#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>


int getNameByPid(DWORD dwProcessId)
{
	HANDLE pProcess = NULL;
	TCHAR szPath[MAX_PATH];
	PDWORD lpdwNameSize = NULL;

	pProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
	if (!pProcess)
	{
		printf("[-] PID: %d\t\tERROR: %d\n", dwProcessId, GetLastError());
		return EXIT_FAILURE;
	}

	if (!GetProcessImageFileNameW(pProcess, szPath, MAX_PATH))
	{
		printf("[-] PID: %d\t\tERROR: %d\n", dwProcessId, GetLastError());
		return EXIT_FAILURE;
	}


	TCHAR* lastSlash = _tcsrchr(szPath, _T('\\'));
	if (lastSlash != NULL)
	{
		// Move a posição após o último "/" para o início da string
		lastSlash++;
	}

	printf("[+] PID: %d\t\tNAME: %S\n", dwProcessId, lastSlash);

	return EXIT_SUCCESS;
}

int main(void)
{
	DWORD aProcesses[1024]; /* Array that receives a list of process ID's */
	DWORD cbNeeded;
	DWORD cProcesses;

	BOOL enumProcesses = EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

	if (!enumProcesses)
	{
		printf("[-] Failed to enum processes.\nError: %d\n", GetLastError());
		return EXIT_FAILURE;
	}
	printf("[+] Sucess to enum processes.\n");

	cProcesses = cbNeeded / sizeof(DWORD);

	for (int i = 0; i < cProcesses; i++)
	{
		getNameByPid(aProcesses[i]);
	}
	//getNameByPid(13824);

	return EXIT_SUCCESS;
}