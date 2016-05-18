#pragma once

#include "mocca/fs/Path.h"

#include <string>

namespace scirunserver {
    void executeSCIRun(const mocca::fs::Path& binaryPath, const std::string& args);
}