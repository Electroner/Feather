#include "pathwork.h"

std::string getCurrentPath()
{
    std::string _path = std::filesystem::current_path().string();
    #ifdef DEBUG
        std::cout << "Path: " << _path << std::endl;
    #endif // DEBUG
		
    return _path;
}

std::string getNormalizedPath()
{
    std::string _path = getCurrentPath();
    std::replace(_path.begin(), _path.end(), '\\', '/');
    #ifdef DEBUG
        std::cout << "Normalized Path: " << _path << std::endl;
    #endif // DEBUG
		
    return _path;
}

#ifdef WINDOWS_SO
std::string browse(HWND hwnd)
{
	std::string path(MAX_PATH, '\0');
	OPENFILENAME ofn = { sizeof(OPENFILENAME) };
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter =
		"All files\0*.*\0"
		"Image files (*.png)\0*.jpg\0";
	ofn.lpstrFile = &path[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
	{
		path.resize(strlen(path.c_str()));
	}
	std::replace(path.begin(), path.end(), '\\', '/');
	return path;
}
#endif

#ifdef LINUX_SO
string browse() {
	string path;
	char filename[1024];
	FILE* f = popen("zenity --file-selection", "r");
	fgets(filename, 1024, f);
	path = filename;
	uid_t permisos = geteuid();
	if (permisos == 0) {
		cout << "File needs to be executed as sudo." << endl;
		chmod(path.c_str(), 0700);
		exit(0);
	}
	return path;
}
#endif
