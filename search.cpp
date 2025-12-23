
#include "allFunctions.h"
bool recursiveSearch(const string &path, const string &target)
{
    DIR *dir = opendir(path.c_str());
    if (!dir)
        return false;

    struct dirent *data;
    while ((data = readdir(dir)) != NULL)
    {
        string name = data->d_name;

        if (name == "." || name == "..")
            continue;

        string fullPath = path + "/" + name;

        if (name == target)
        {
            closedir(dir);
            return true;
        }

        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
        {
            if (recursiveSearch(fullPath, target))
            {
                closedir(dir);
                return true;
            }
        }
    }
    closedir(dir);
    return false;
}

void runSearch(const vector<string> &args)
{
    if (args.size() != 2)
    {
        cerr << "Usage: search <filename>\n";
        return;
    }

    char cwd[4096];
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd");
        return;
    }

    bool isPresent = recursiveSearch(cwd, args[1]);
    cout << (isPresent ? "True" : "False") << endl;
}
