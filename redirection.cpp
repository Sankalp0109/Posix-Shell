

#include "allFunctions.h"
void handleRedirections(vector<string> &cmd)
{
    for (int i = 0; i < cmd.size();)
    {
        if (cmd[i] == "<")
        {
            int fd = open(cmd[i + 1].c_str(), O_RDONLY);
            if (fd < 0)
            {
                perror("open < failed");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2);
        }
        else if (cmd[i] == ">")
        {
            int fd = open(cmd[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open > failed");
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2);
        }
        else if (cmd[i] == ">>")
        {
            int fd = open(cmd[i + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("open >> failed");
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2);
        }
        else
        {
            i++;
        }
    }
}