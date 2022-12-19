#ifndef PATHWORK
#define PATHWORK

#include "../GUI/feathergui.h"

std::string getCurrentPath();

std::string getCurrentNormalizedPath();

std::string getNormalizedPath(std::string _path);

#ifdef WINDOWS_SO
std::string browseFile(HWND hwnd);
std::string browseFolder(HWND hwnd, int _option);
#endif

#ifdef LINUX_SO
std::string browse();
#endif


#endif