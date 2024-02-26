#include <windows.h>
#include <stdio.h>

int main()
{
    HMODULE hModule = LoadLibrary("msgboxdll.dll");

    if (hModule != NULL)
    {
        printf("[+] DLL Carregada com sucesso!\n");
        FreeLibrary(hModule);
        return EXIT_SUCCESS;
    } else
    {
        printf("[-] Falha ao carregar a DLL\n");
    }

    return EXIT_FAILURE;
}