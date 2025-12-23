
#include "allFunctions.h"

void printCd(const vector<string> &args, string &homeDir)
{
    string path;
    if (args.size() == 1)
        path = homeDir;
    else
        path = args[1];
    cout << path << endl;
    if (chdir(path.c_str()) != 0)
        perror("cd");
}
