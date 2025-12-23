
#include "allFunctions.h"
bool isBuiltin(vector<string> commands)
{
    string cmd = commands[0];

    return (cmd == "echo" || cmd == "cd" || cmd == "pwd" || cmd == "ls" || cmd == "history" || cmd == "pinfo" || cmd == "search" || (commands.size() >= 2 && commands[1] == "&"));
}
vector<vector<string>> splitPipeline(const vector<string> &tokens)
{
    vector<vector<string>> commands;
    vector<string> current;

    for (auto &t : tokens)
    {
        if (t == "|")
        {
            if (!current.empty())
            {
                commands.push_back(current);
                current.clear();
            }
        }
        else
        {
            current.push_back(t);
        }
    }
    if (!current.empty())
        commands.push_back(current);

    return commands;
}

void runBuiltin(vector<string> &cmd, string &homeDir)
{

    if (cmd.empty())
        return;

    if (cmd[0] == "echo")
    {
        printEcho(cmd);
    }
    else if (cmd[0] == "cd")
    {
        printCd(cmd, homeDir);
    }
    else if (cmd[0] == "ls")
    {
        listFile(cmd, homeDir);
    }
    else if (cmd[0] == "pinfo")
    {
        pinfo(cmd);
    }
    else if (cmd[0] == "search")
    {
        runSearch(cmd);
    }
    else if (cmd[0] == "pwd")
    {
        printPwd();
    }
    else if (cmd[0] == "history")
    {
        if (cmd.size() == 2)
        {
            printHistory(stoi(cmd[1]));
        }
        else
        {
            printHistory(10);
        }
    }
    else if (cmd.size() >= 2 && cmd[1] == "&")
    {
        startProcess(cmd);
    }
}

void executePipeline(const vector<string> &tokens, string &homeDir)
{
    vector<vector<string>> commands = splitPipeline(tokens);
    int n = commands.size();

    vector<int> pfd(2);
    int prev_fd = -1;

    for (int i = 0; i < n; i++)
    {
        if (i < n - 1)
        {
            if (pipe(pfd.data()) < 0)
            {
                perror("pipe");
            }
        }

        pid_t pid = fork();
        if (pid == 0)
        {

            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < n - 1)
            {
                close(pfd[0]);
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }

            handleRedirections(commands[i]);

            if (isBuiltin(commands[i]))
            {
                runBuiltin(commands[i], homeDir);
                exit(0);
            }

            vector<char *> argv;
            for (auto &s : commands[i])
                argv.push_back(const_cast<char *>(s.c_str()));
            argv.push_back(NULL);

            if (execvp(argv[0], argv.data()) < 0)
            {
                perror("execvp failed");
                exit(1);
            }
        }
        else if (pid > 0)
        {

            if (prev_fd != -1)
                close(prev_fd);
            if (i < n - 1)
            {
                close(pfd[1]);
                prev_fd = pfd[0];
            }
        }
        else
        {
            perror("fork failed");
        }
    }

    for (int i = 0; i < n; i++)
        wait(NULL);
}