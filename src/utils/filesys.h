#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "filestruct.h"

class FileStream {
public:
	void File(const std::filesystem::path &FileLocation, ContentFiles &Files);
private:
    std::ifstream InFile;
    std::ofstream OutFile;
};