#include "allFunctions.h"

// Converts file mode bits to a permission string like "-rwxr-xr-x"
string permissions(mode_t mode)
{
    string permission;
    permission += (S_ISDIR(mode)) ? 'd' : '-';
    permission += (mode & S_IRUSR) ? 'r' : '-';
    permission += (mode & S_IWUSR) ? 'w' : '-';
    permission += (mode & S_IXUSR) ? 'x' : '-';
    permission += (mode & S_IRGRP) ? 'r' : '-';
    permission += (mode & S_IWGRP) ? 'w' : '-';
    permission += (mode & S_IXGRP) ? 'x' : '-';
    permission += (mode & S_IROTH) ? 'r' : '-';
    permission += (mode & S_IWOTH) ? 'w' : '-';
    permission += (mode & S_IXOTH) ? 'x' : '-';
    return permission;
}

// Prints file information in long format (permissions, links, owner, size, date, name)
void print_long(const string &path, const string &name)
{
    struct stat st{};
    string addPath = path + "/" + name;
    if (stat(addPath.c_str(), &st) == -1)
    {
        perror("stat");
        return;
    }

    cout << permissions(st.st_mode) << " ";
    cout << setw(2) << st.st_nlink << " ";

    struct passwd *pw = getpwuid(st.st_uid);
    cout << (pw ? pw->pw_name : to_string(st.st_uid)) << " ";

    struct group *gr = getgrgid(st.st_gid);
    cout << (gr ? gr->gr_name : to_string(st.st_gid)) << " ";

    cout << setw(6) << st.st_size << " ";

    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st.st_mtime));
    cout << timebuf << " ";

    cout << name << endl;
}

// Lists all entries in a directory with optional flags for hidden files and long format
void printFolderData(const string &path, bool show_all, bool long_format)
{
    DIR *dir = opendir(path.c_str());
    if (!dir)
    {
        perror(("ls: cannot access " + path).c_str());
        return;
    }

    vector<string> entries;
    struct dirent *data;
    while ((data = readdir(dir)) != nullptr)
    {
        string name = data->d_name;
        if (!show_all && name[0] == '.')
            continue;
        entries.push_back(name);
    }
    closedir(dir);

    sort(entries.begin(), entries.end());

    if (long_format)
    {
        for (const auto &name : entries)
            print_long(path, name);
    }
    else
    {
        for (const auto &name : entries)
            cout << name << endl;
    }
}

// Prints information for a single file
void printFileData(const string &path, bool long_format)
{
    struct stat st{};
    if (stat(path.c_str(), &st) == -1)
    {
        perror(("ls: cannot access " + path).c_str());
        return;
    }

    string name = path.substr(path.find_last_of("/\\") + 1);
    if (long_format)
    {
        print_long(".", path);
    }
    else
    {
        cout << name << endl;
    }
}

// Main ls command handler supporting -a, -l flags and multiple paths
int listFile(const vector<string> &args, string &homeDir)
{
    bool showAll = false, longFormat = false;
    vector<string> paths;
    for (int i = 1; i < args.size(); i++)
    {
        string flag = args[i];
        if (flag[0] == '-')
        {
            if (flag.find('a') != -1)
                showAll = true;
            if (flag.find('l') != -1)
                longFormat = true;
        }
        else
        {
            if (flag == "~")
            {
                paths.push_back(homeDir);
            }
            else
            {
                paths.push_back(flag);
            }
        }
    }
    if (paths.size() == 0)
        paths.push_back(".");
    for (int i = 0; i < paths.size(); i++)
    {
        struct stat st;
        if (stat(paths[i].c_str(), &st) == -1)
        {
            perror(("ls: cannot access: " + paths[i]).c_str());
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            if (paths.size() > 1)
                cout << paths[i] << ":" << endl;
            printFolderData(paths[i], showAll, longFormat);
            if (i < paths.size() - 1)
                cout << endl;
        }
        else
        {
            printFileData(paths[i], longFormat);
        }
    }
    return 0;
}
