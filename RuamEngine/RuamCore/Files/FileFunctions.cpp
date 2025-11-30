#include "FileFunctions.h"
#include <string>
#include <vector>

// This function reads a file and returns its content as a string.
std::string FileToString(const std::string& absoluteFilePath)
{
	std::ifstream file(absoluteFilePath);
	if (!file.is_open()) std::cout << "ERROR WHILE TRYING TO OPEN " << absoluteFilePath << "\n";
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

// This function reads a file from the project root directory and returns its content as a string.
std::string RelativeFileToString(const std::string& relativeFilePath)
{
	return FileToString(GlobalizePath(relativeFilePath));
}

// Relative from the RuamEngine root directory
std::string GlobalizePath(const std::string& relativeFilePath)
{
	return std::string(PROJECT_ROOT_DIR) + "/" + relativeFilePath;
}

std::vector<std::string> GlobalizePaths(const std::vector<std::string>& relativeFilePaths)
{
    std::vector<std::string> globalizedPaths;
    for (auto& path : relativeFilePaths)
    {
        globalizedPaths.push_back(GlobalizePath(path));
    }
	return globalizedPaths;
}


std::string RelativizePath(const std::string& absoluteFilePath)
{
	return std::filesystem::relative(absoluteFilePath, std::string(PROJECT_ROOT_DIR)).string();
}
