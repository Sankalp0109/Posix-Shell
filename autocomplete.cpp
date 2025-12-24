#include "allFunctions.h"

static const char *builtins[] = {
    "cd", "echo", "pwd", "ls", "history", "pinfo", "search", "exit", NULL
};

static vector<string> matches;
static size_t match_index;

// Returns all executable commands from PATH and builtins matching the given prefix
vector<string> getPathCommands(const char *text)
{
    vector<string> commands;
    string prefix(text);
    
    for (int i = 0; builtins[i] != NULL; i++)
    {
        string cmd = builtins[i];
        if (cmd.find(prefix) == 0)
        {
            commands.push_back(cmd);
        }
    }
    
    const char *path_env = getenv("PATH");
    if (!path_env)
        return commands;
    
    string path_str(path_env);
    stringstream ss(path_str);
    string dir;
    
    while (getline(ss, dir, ':'))
    {
        DIR *dp = opendir(dir.c_str());
        if (!dp)
            continue;
        
        struct dirent *entry;
        while ((entry = readdir(dp)) != NULL)
        {
            string name = entry->d_name;
            if (name.find(prefix) == 0)
            {
                string full_path = dir + "/" + name;
                struct stat st;
                if (stat(full_path.c_str(), &st) == 0 && (st.st_mode & S_IXUSR))
                {
                    if (find(commands.begin(), commands.end(), name) == commands.end())
                    {
                        commands.push_back(name);
                    }
                }
            }
        }
        closedir(dp);
    }
    
    sort(commands.begin(), commands.end());
    return commands;
}

// Returns matching files and directories for the given prefix
vector<string> getFileMatches(const char *text)
{
    vector<string> files;
    string prefix(text);
    string dir_path = ".";
    string file_prefix = prefix;
    
    size_t last_slash = prefix.rfind('/');
    if (last_slash != string::npos)
    {
        dir_path = prefix.substr(0, last_slash);
        if (dir_path.empty())
            dir_path = "/";
        file_prefix = prefix.substr(last_slash + 1);
    }
    
    DIR *dp = opendir(dir_path.c_str());
    if (!dp)
        return files;
    
    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL)
    {
        string name = entry->d_name;
        
        if (name == "." || name == "..")
        {
            if (file_prefix != "." && file_prefix != "..")
                continue;
        }
        
        if (name.find(file_prefix) == 0)
        {
            string full_match;
            if (last_slash != string::npos)
            {
                full_match = dir_path + "/" + name;
            }
            else
            {
                full_match = name;
            }
            
            struct stat st;
            string check_path = dir_path + "/" + name;
            if (stat(check_path.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
            {
                full_match += "/";
            }
            
            files.push_back(full_match);
        }
    }
    closedir(dp);
    
    sort(files.begin(), files.end());
    return files;
}

// Generator function for command completion in readline
char *command_generator(const char *text, int state)
{
    if (state == 0)
    {
        matches = getPathCommands(text);
        match_index = 0;
    }
    
    if (match_index < matches.size())
    {
        return strdup(matches[match_index++].c_str());
    }
    
    return NULL;
}

// Generator function for file completion in readline
char *file_generator(const char *text, int state)
{
    if (state == 0)
    {
        matches = getFileMatches(text);
        match_index = 0;
    }
    
    if (match_index < matches.size())
    {
        return strdup(matches[match_index++].c_str());
    }
    
    return NULL;
}

// Main completion function that decides between command and file completion
char **shell_completion(const char *text, int start, int end)
{
    (void)end;
    
    rl_attempted_completion_over = 1;
    
    char **result = NULL;
    
    if (start == 0)
    {
        result = rl_completion_matches(text, command_generator);
    }
    else
    {
        result = rl_completion_matches(text, file_generator);
    }
    
    return result;
}

// Initializes the autocomplete system for the shell
void setupAutocomplete()
{
    rl_attempted_completion_function = shell_completion;
    rl_basic_word_break_characters = (char *)" \t\n\"\\'`@$><=;|&{(";
}
