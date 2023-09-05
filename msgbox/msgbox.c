#include <windows.h>

int main(void)
{
    MessageBoxW
    (
        NULL,
        L"My first message box!",
        L"Message box Title",
        MB_ICONERROR | MB_OK
    );


    return 0;
}