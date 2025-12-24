#include "allFunctions.h"

using namespace std;

#define HISTORY_FILE ".myshell_history"
#define MAX_HISTORY 20

// Returns the full path to the history file in the user's home directory
string getHistoryFilePath()
{
    const char *home = getenv("HOME");
    if (!home)
        home = ".";
    return string(home) + "/" + HISTORY_FILE;
}

// Trims the history to keep only the most recent entries up to max limit
void trimHistory(int max)
{
    while (history_length > max)
    {
        remove_history(0);
    }
}

// Loads command history from the history file
void initHistory()
{
    string histFile = getHistoryFilePath();
    read_history(histFile.c_str());
    trimHistory(MAX_HISTORY);
}

// Saves the current command history to the history file
void saveHistory()
{
    string histFile = getHistoryFilePath();
    trimHistory(MAX_HISTORY);
    write_history(histFile.c_str());
}

// Prints the last 'num' commands from history
void printHistory(int num)
{
    int total = history_length;
    if (total == 0)
        return;

    int start = max(0, total - num);

    for (int i = start; i < total; i++)
    {
        HIST_ENTRY *entry = history_get(i + history_base);
        if (entry)
            cout << entry->line << "\n";
    }
}
