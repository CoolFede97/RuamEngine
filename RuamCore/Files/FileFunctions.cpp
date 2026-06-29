#include "FileFunctions.h"
#include <string>
#include <vector>

// This function reads a file and returns its content as a string.
std::string fileToString(const std::string& absoluteFilePath)
{
	std::ifstream file(absoluteFilePath);
	if (!file.is_open()) std::cout << "ERROR WHILE TRYING TO OPEN " << absoluteFilePath << "\n";
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

// This function reads a file from the project root directory and returns its content as a string.
std::string relativefileToString(const std::string& relativeFilePath)
{
	return fileToString(globalizePath(relativeFilePath));
}

// Relative from the RuamEngine root directory
std::string globalizePath(const std::string& relativeFilePath)
{
	return std::string(PROJECT_ROOT_DIR) + "/" + relativeFilePath;
}

std::vector<std::string> globalizePaths(const std::vector<std::string>& relativeFilePaths)
{
    std::vector<std::string> globalizedPaths;
    for (auto& path : relativeFilePaths)
    {
        globalizedPaths.push_back(globalizePath(path));
    }
	return globalizedPaths;
}

std::string unifyPaths(const std::vector<std::string>& relativePaths)
{
    std::string unifiedPath = "";
    for (unsigned int i = 0; i < relativePaths.size(); i++)
    {
        unifiedPath+=relativePaths[i];
        if (i!=relativePaths.size()-1) unifiedPath += "|-|";
    }
    return unifiedPath;
}

std::string relativizePath(const std::string& absoluteFilePath)
{
	return std::filesystem::relative(absoluteFilePath, std::string(PROJECT_ROOT_DIR)).string();
}
