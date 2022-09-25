#ifndef PATHWORK
#define PATHWORK

#define WINDOWS_SO
//#define LINUX_SO

#ifdef WINDOWS_SO
#include <windows.h>
#include <shlobj_core.h>
#include <commdlg.h>
#endif 

#ifdef LINUX
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

std::string getCurrentPath();

std::string getNormalizedPath();

#ifdef WINDOWS_SO
std::string browseFile(HWND hwnd);
std::string browseFolder(HWND hwnd);
#endif

#ifdef LINUX_SO
std::string browse();
#endif


#endif