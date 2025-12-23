
#include "allFunctions.h"
extern pid_t pid;
void startProcess(vector<string> &args)
{
    if (args.empty())
        return;

    bool bg = false;

    if (args.back() == "&")
    {
        bg = true;
        args.pop_back();
    }

    vector<char *> arr;
    for (int i = 0; i < args.size(); i++)
        arr.push_back((char *)args[i].c_str());
    arr.push_back(nullptr);

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        if (execvp(arr[0], arr.data()) < 0)
        {
            cout << arr[0] << ": command not found" << endl;
        }
    }
    else
    {
        if (bg)
        {
            cout << "Started background process PID: " << pid << endl;
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}