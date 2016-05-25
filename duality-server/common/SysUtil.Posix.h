#pragma once

#include "mocca/fs/Path.h"

#include <string>
#include <vector>

class SysUtilImpl {
public:
    static void execute(const mocca::fs::Path& binary, const std::vector<std::string>& args);
};