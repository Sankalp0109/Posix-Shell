#include "allFunctions.h"

// Prints the given arguments to stdout with redirection support
void printEcho(vector<string> cmd)
{
    int savedStdout = dup(STDOUT_FILENO);

    handleRedirections(cmd);

    string output;
    for (int i = 1; i < cmd.size(); i++)
    {
        output += cmd[i];
        if (i + 1 < cmd.size())
            output += " ";
    }
    output += "\n";

    if (write(STDOUT_FILENO, output.c_str(), output.size()) < 0)
    {
        perror("echo write failed");
    }

    dup2(savedStdout, STDOUT_FILENO);
    close(savedStdout);
}
