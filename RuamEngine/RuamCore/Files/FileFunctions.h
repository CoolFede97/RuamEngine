#pragma once

#include <string>

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

std::string FileToString(const std::string& absoluteFilePath);

std::string RelativeFileToString(const std::string& relativeFilePath);

std::string GlobalizePath(const std::string& relativeFilePath);

std::string RelativizePath(const std::string& absoluteFilePath);