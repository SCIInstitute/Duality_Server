#pragma once

#include "mocca/fs/Path.h"

#include <fstream>
#include <vector>

class TempFile {
public:
    TempFile(const mocca::fs::Path& path, const std::string& text);
    ~TempFile();

private:
    mocca::fs::Path path_;
    std::ofstream file_;
};

class SysUtil {
public:
    static void execute(const mocca::fs::Path& binary, const std::vector<std::string> args);
};

