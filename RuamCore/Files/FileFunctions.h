#pragma once

#include <string>

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <vector>

std::string FileToString(const std::string& absoluteFilePath);

std::string RelativeFileToString(const std::string& relativeFilePath);

std::string GlobalizePath(const std::string& relativeFilePath);

std::vector<std::string> GlobalizePaths(const std::vector<std::string>& relativeFilePaths);

std::string UnifyPaths(const std::vector<std::string>& relativePaths);

std::string RelativizePath(const std::string& absoluteFilePath);
