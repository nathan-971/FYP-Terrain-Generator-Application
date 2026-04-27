#include "utils/resources.h"
#include <Windows.h>

std::filesystem::path Resources::AssetPath(const std::string& resourceDirectory)
{
	return getWorkingDirectory() / "assets" / resourceDirectory;
}

std::filesystem::path Resources::getWorkingDirectory()
{
	char buffer[_MAX_PATH];
	GetModuleFileNameA(NULL, buffer, _MAX_PATH);
	return std::filesystem::path(buffer).parent_path();
}