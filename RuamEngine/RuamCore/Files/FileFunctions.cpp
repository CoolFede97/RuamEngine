#include "FileFunctions.h"

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

std::string RelativizePath(const std::string& absoluteFilePath)
{
	return std::filesystem::relative(absoluteFilePath, std::string(PROJECT_ROOT_DIR)).string();
}

