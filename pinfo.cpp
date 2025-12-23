
#include "allFunctions.h"

using namespace std;

void pinfo(const vector<string> &cmd)
{
    pid_t pid;

    if (cmd.size() == 1)
        pid = getpid();
    else
        pid = stoi(cmd[1]);

    string statPath = "/proc/" + to_string(pid) + "/stat";
    int fd = open(statPath.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("open stat");
        return;
    }

    char buffer[4096];
    unsigned long bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);

    if (bytes <= 0)
    {
        cerr << "pinfo: failed to read stat for pid " << pid << "\n";
        return;
    }
    buffer[bytes] = '\0';

    stringstream ss(buffer);
    int pid_dummy;
    string comm, state;
    unsigned long virtualMemory;
    string dummy;

    ss >> pid_dummy >> comm >> state;
    for (int i = 0; i < 19; i++)
        ss >> dummy;
    ss >> virtualMemory;

    string exePath = "/proc/" + to_string(pid) + "/exe";
    char buff[4096];
    unsigned long len = readlink(exePath.c_str(), buff, sizeof(buff) - 1);
    string exe;
    if (len != -1)
    {
        buff[len] = '\0';
        exe = string(buff);
    }
    else
    {
        exe = "Executable path not found";
    }

    cout << "pid -- " << pid << "\n";
    cout << "Process Status -- " << state << "\n";
    cout << "memory -- " << virtualMemory << " {Virtual Memory}\n";
    cout << "Executable Path -- " << exe << "\n";
}
