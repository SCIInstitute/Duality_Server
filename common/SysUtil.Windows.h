#pragma once

#include "mocca/fs/Path.h"

#include <Windows.h>

#include <string>
#include <vector>

class SysUtilImpl {
public:
    static void execute(const mocca::fs::Path& binary, const std::vector<std::string>& args);

private:
    static std::string getWindowsError();
    
    struct TempHandles {
        TempHandles();
        ~TempHandles();

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
    };
};

