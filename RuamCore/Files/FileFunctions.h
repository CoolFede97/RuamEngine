#pragma once

#include <string>

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <vector>

std::string fileToString(const std::string& absoluteFilePath);

std::string relativefileToString(const std::string& relativeFilePath);

std::string globalizePath(const std::string& relativeFilePath);

std::vector<std::string> globalizePaths(const std::vector<std::string>& relativeFilePaths);

std::string unifyPaths(const std::vector<std::string>& relativePaths);

std::string relativizePath(const std::string& absoluteFilePath);
