
#include "allFunctions.h"

pid_t pid = -1;

void sigintHandler(int sig)
{
    if (pid > 0)
    {
        kill(pid, SIGINT);
    }
}

void sigtstpHandler(int sig)
{
    if (pid > 0)
    {
        kill(pid, SIGTSTP);
    }
}

void setupSignals()
{
    signal(SIGINT, sigintHandler);
    signal(SIGTSTP, sigtstpHandler);
}

int main()
{

    char start[4096];
    if (!getcwd(start, sizeof(start)))
    {
        perror("getcwd");
        return 1;
    }
    string homeDir = string(start);

    struct passwd *pw = getpwuid(getuid());
    string user = pw ? pw->pw_name : "user";
    char hostbuf[256];
    gethostname(hostbuf, sizeof(hostbuf));
    string host = hostbuf;

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    initHistory();
    setupSignals();

    while (true)
    {
        char cwdBuf[4096];
        if (!getcwd(cwdBuf, sizeof(cwdBuf)))
            strcpy(cwdBuf, "?");
        string disp = tildeDisplay(cwdBuf, homeDir);

        string prompt = user + "@" + host + ":" + disp + "$ ";
        char *in = readline(prompt.c_str());
        if (!in)
        {
            cout << "\n";
            break;
        }

        string line(in);
        free(in);

        if (all_of(line.begin(), line.end(),
                   [](unsigned char ch)
                   { return isspace(ch); }))
            continue;

        add_history(line.c_str());
        saveHistory();

        vector<string> cmd = semicolonSplit(line);
        for (auto &x : cmd)
        {
            vector<string> args = tokenize(x);
            if (args.empty())
                continue;
            for (auto &a : args)
                a = tildeExpand(a, homeDir);

            if (args[0] == "exit")
            {
                saveHistory();
                return 0;
            }

            executePipeline(args, homeDir);
        }
    }
    saveHistory();

    return 0;
}
