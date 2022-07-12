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
