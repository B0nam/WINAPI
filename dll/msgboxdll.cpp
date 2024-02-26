#include <windows.h>

#pragma comment (lib, "user32.lib")

BOOL APIENTRY WINAPI DllMain(
    HMODULE hModule,        // IDENTIFICADOR PARA O MODULO DLL
    DWORD nReason,          // CODIGO COM O MOTIVO DA CHAMADA DA DLL
    LPVOID lpReserverd)     // ARGUMENTO DE CONTROLE (DLL CARREGADA COMO ESTATICA OU DINAMICA)
{
    switch (nReason)
    {
    case DLL_PROCESS_ATTACH:
        MessageBoxW(NULL, L"DLL_PROCESS_ATTACH", NULL, MB_OK);
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    
    default:
        break;
    }

    return EXIT_SUCCESS;
}