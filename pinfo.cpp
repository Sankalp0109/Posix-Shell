#include "allFunctions.h"

using namespace std;

#ifdef __APPLE__
// Displays process information (pid, status, memory, executable path) on macOS
void pinfo(const vector<string> &cmd)
{
    pid_t pid;

    if (cmd.size() == 1)
        pid = getpid();
    else
        pid = stoi(cmd[1]);

    struct proc_bsdinfo proc;
    int size = proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc, sizeof(proc));
    
    if (size <= 0)
    {
        cerr << "pinfo: failed to get info for pid " << pid << "\n";
        return;
    }

    string state;
    switch (proc.pbi_status)
    {
        case SRUN:
            state = "R";
            break;
        case SSLEEP:
            state = "S";
            break;
        case SSTOP:
            state = "T";
            break;
        case SZOMB:
            state = "Z";
            break;
        default:
            state = "?";
    }
    
    if (proc.pbi_flags & PROC_FLAG_CONTROLT)
    {
        state += "+";
    }

    struct proc_taskinfo taskInfo;
    int taskSize = proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &taskInfo, sizeof(taskInfo));
    unsigned long virtualMemory = 0;
    if (taskSize > 0)
    {
        virtualMemory = taskInfo.pti_virtual_size / 1024;
    }

    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    string exe;
    if (proc_pidpath(pid, pathbuf, sizeof(pathbuf)) > 0)
    {
        exe = string(pathbuf);
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

#else
// Displays process information (pid, status, memory, executable path) on Linux
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
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
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
    ssize_t len = readlink(exePath.c_str(), buff, sizeof(buff) - 1);
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
#endif
