#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include<stdio.h>
#include <string>
#include <vector>
#include <algorithm>   
#include <cctype>      
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <sstream> 
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <readline/readline.h>
#include <readline/history.h>
using namespace std;

string tildeExpand(const string &path, const string &homeDir);
string tildeDisplay(const string &arg, const string &homeDir);
vector<string> tokenize(const string &input);
vector<string> semicolonSplit(const string &input);
void printEcho(vector<string> cmd);
void handleRedirections(vector<string> &cmd);
void pinfo(const vector<string> &cmd);
void printCd(const vector<string> &args, string &homeDir);
void printHistory(int num = 10);
string getHistoryFilePath();
void trimHistory(int max);
void initHistory();
void saveHistory();
void executePipeline(const vector<string> &tokens, string &homeDir);
void printCd(const vector<string> &args, string &homeDir);
void runSearch(const vector<string> &args);
int listFile(const vector<string> &args, string &homeDir);
void printPwd();
void startProcess(vector<string> &args);
#endif
