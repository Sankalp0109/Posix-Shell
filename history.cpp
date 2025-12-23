
#include "allFunctions.h"
using namespace std;
#define HISTORY_FILE ".myshell_history"
#define MAX_HISTORY 20
string getHistoryFilePath()
{
    const char *home = getenv("HOME");
    if (!home)
        home = ".";
    return string(home) + "/" + HISTORY_FILE;
}

void trimHistory(int max)
{
    while (history_length > max)
    {
        remove_history(0);
    }
}

void initHistory()
{
    string histFile = getHistoryFilePath();
    read_history(histFile.c_str());
    trimHistory(MAX_HISTORY);
}

void saveHistory()
{
    string histFile = getHistoryFilePath();
    trimHistory(MAX_HISTORY);
    write_history(histFile.c_str());
}

void printHistory(int num)
{
    HIST_ENTRY **histList = history_list();
    if (!histList)
        return;

    int total = history_length;
    int start = max(0, total - num);

    for (int i = start; i < total; i++)
    {
        cout << histList[i]->line << "\n";
    }
}