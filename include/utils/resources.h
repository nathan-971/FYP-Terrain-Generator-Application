#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <filesystem>

class Resources
{
public:
	static std::filesystem::path AssetPath(const std::string& resourceDirectory);
private:
	static std::filesystem::path getWorkingDirectory();
};

#endif