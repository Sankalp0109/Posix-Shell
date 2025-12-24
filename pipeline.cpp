#include "allFunctions.h"

// Checks if command must run in parent process (modifies shell state)
bool isParentBuiltin(const string &cmd)
{
    return (cmd == "cd" || cmd == "exit");
}

// Checks if command can run in child process (only produces output)
bool isChildBuiltin(const string &cmd)
{
    return (cmd == "echo" || cmd == "pwd" || cmd == "ls" || 
            cmd == "history" || cmd == "pinfo" || cmd == "search");
}

// Splits command tokens by pipe symbols into separate commands
vector<vector<string>> splitPipeline(const vector<string> &tokens)
{
    vector<vector<string>> commands;
    vector<string> current;

    for (const auto &t : tokens)
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

// Executes a builtin command based on its name
void runBuiltin(vector<string> &cmd, string &homeDir)
{
    if (cmd.empty())
        return;

    if (cmd[0] == "echo")
    {
        printEcho(cmd);
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
}

// Executes a pipeline of commands with proper pipe and fork handling
void executePipeline(const vector<string> &tokens, string &homeDir)
{
    vector<vector<string>> commands = splitPipeline(tokens);
    size_t n = commands.size();

    if (n == 0 || commands[0].empty())
        return;

    if (n == 1)
    {
        const string &cmd = commands[0][0];
        
        if (cmd == "cd")
        {
            printCd(commands[0], homeDir);
            return;
        }
        
        if (commands[0].size() >= 2 && commands[0].back() == "&")
        {
            startProcess(commands[0]);
            return;
        }
        
        if (isChildBuiltin(cmd))
        {
            int savedStdin = dup(STDIN_FILENO);
            int savedStdout = dup(STDOUT_FILENO);
            
            handleRedirections(commands[0]);
            runBuiltin(commands[0], homeDir);
            
            dup2(savedStdin, STDIN_FILENO);
            dup2(savedStdout, STDOUT_FILENO);
            close(savedStdin);
            close(savedStdout);
            return;
        }
    }

    vector<pid_t> pids(n);
    int prev_fd = -1;

    for (size_t i = 0; i < n; i++)
    {
        int pfd[2] = {-1, -1};
        
        if (i < n - 1)
        {
            if (pipe(pfd) < 0)
            {
                perror("pipe");
                return;
            }
        }

        pids[i] = fork();
        
        if (pids[i] == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (pfd[1] != -1)
            {
                close(pfd[0]);
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }

            handleRedirections(commands[i]);

            if (isChildBuiltin(commands[i][0]))
            {
                runBuiltin(commands[i], homeDir);
                _exit(0);
            }

            vector<char *> argv;
            for (auto &s : commands[i])
                argv.push_back(const_cast<char *>(s.c_str()));
            argv.push_back(nullptr);

            execvp(argv[0], argv.data());
            perror("execvp failed");
            _exit(1);
        }
        else if (pids[i] > 0)
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (pfd[1] != -1)
                close(pfd[1]);
            prev_fd = pfd[0];
        }
        else
        {
            perror("fork failed");
            return;
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
    }
}
