#include "allFunctions.h"

static string prevDir;

// Changes the current working directory with support for ~, -, and relative/absolute paths
void printCd(const vector<string> &args, string &homeDir)
{
    if (prevDir.empty())
    {
        char buf[4096];
        if (getcwd(buf, sizeof(buf)))
            prevDir = string(buf);
    }

    if (args.size() > 2)
    {
        cerr << "cd: Invalid arguments\n";
        return;
    }

    string target;
    if (args.size() == 1)
    {
        target = homeDir;
    }
    else if (args[1] == "-")
    {
        if (prevDir.empty())
        {
            cerr << "cd: OLDPWD not set\n";
            return;
        }
        target = prevDir;
    }
    else if (args[1] == "~")
    {
        target = homeDir;
    }
    else
    {
        target = args[1];
    }

    char oldBuf[4096];
    string oldDir;
    if (getcwd(oldBuf, sizeof(oldBuf)))
        oldDir = string(oldBuf);

    if (chdir(target.c_str()) != 0)
    {
        perror("cd");
        return;
    }

    if (args.size() > 1 && args[1] == "-")
    {
        char newBuf[4096];
        if (getcwd(newBuf, sizeof(newBuf)))
            cout << newBuf << endl;
    }

    prevDir = oldDir;
}
