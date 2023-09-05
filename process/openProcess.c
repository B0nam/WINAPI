#include <windows.h>
#include <stdio.h>

int main(void)
{
    HANDLE myProcessHandler;

    if(!OpenProcess(STANDARD_RIGHTS_ALL,FALSE,16004))
    {
        printf("(-) Process cannot be opened.\n");
        return EXIT_FAILURE;
    }
    printf("(+) The Process got opened.\n");

    return EXIT_SUCCESS;
}