
#include "allFunctions.h"

string tildeDisplay(const string &path, const string &homeDir)
{
    if (path == homeDir)
        return "~";
    if (!homeDir.empty() && path.rfind(homeDir + "/", 0) == 0)
        return "~" + path.substr(homeDir.size());
    return path;
}

string tildeExpand(const string &arg, const string &homeDir)
{
    if (arg == "~")
        return homeDir;
    if (arg.size() >= 2 && arg[0] == '~' && arg[1] == '/')
        return homeDir + arg.substr(1);
    return arg;
}

vector<string> tokenize(const string &command)
{
    vector<string> tokens;
    string cur;
    bool inSingle = false, inDouble = false;

    for (int i = 0; i < command.size(); ++i)
    {
        char c = command[i];

        if (inSingle)
        {
            if (c == '\'')
                inSingle = false;
            else
                cur.push_back(c);
        }
        else if (inDouble)
        {
            if (c == '"')
                inDouble = false;
            else if (c == '\\' && i + 1 < command.size())
                cur.push_back(command[++i]);
            else
                cur.push_back(c);
        }
        else
        {
            if (c == '|' || c == '<' || c == '>')
            {
                if (!cur.empty())
                {
                    tokens.push_back(cur);
                    cur.clear();
                }

                if (c == '>' && i + 1 < command.size() && command[i + 1] == '>')
                {
                    tokens.push_back(">>");
                    i++;
                }
                else
                {
                    tokens.push_back(string(1, c));
                }
            }
            else if (isspace(static_cast<unsigned char>(c)))
            {
                if (!cur.empty())
                {
                    tokens.push_back(cur);
                    cur.clear();
                }
            }
            else if (c == '\'')
            {
                inSingle = true;
            }
            else if (c == '"')
            {
                inDouble = true;
            }
            else
            {
                cur.push_back(c);
            }
        }
    }

    if (!cur.empty())
        tokens.push_back(cur);

    return tokens;
}
vector<string> semicolonSplit(const string &input)
{
    vector<string> commands;
    stringstream ss(input);
    string cmd;

    while (getline(ss, cmd, ';'))
    {
        if (!cmd.empty())
            commands.push_back(cmd);
    }

    return commands;
}
