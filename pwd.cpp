#include "allFunctions.h"

// Prints the current working directory
void printPwd()
{
    char buf[4096];
    if (getcwd(buf, sizeof(buf)))
        cout << buf << endl;
    else
        perror("pwd");
}
