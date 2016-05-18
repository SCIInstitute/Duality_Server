#include "common/SysUtil.Windows.h"

#include "common/Error.h"

#include "mocca/base/StringTools.h"

std::string SysUtilImpl::getWindowsError() {
    // Get the error message, if any
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); // No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
        errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    // Free the buffer
    LocalFree(messageBuffer);

    return message;
}

SysUtilImpl::TempHandles::TempHandles() {
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
}

SysUtilImpl::TempHandles::~TempHandles() {
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void SysUtilImpl::execute(const mocca::fs::Path& binary, const std::vector<std::string> args) {
    auto commandLine = binary.toString() + " " + mocca::joinString(args, " ");
    TempHandles handles;
    if (!CreateProcessA(NULL,                                  // Application name
        const_cast<char*>(commandLine.data()), // Command line
        NULL,                                  // Process handle not inheritable
        NULL,                                  // Thread handle not inheritable
        FALSE,                                 // Set handle inheritance to FALSE
        0,                                     // No creation flags
        NULL,                                  // Use parent's environment block
        NULL,                                  // Use parent's starting directory
        &handles.si,                           // Pointer to STARTUPINFO structure
        &handles.pi)) {
        throw Error("Error starting SCIRun: " + getWindowsError(), __FILE__, __LINE__);
    }
    WaitForSingleObject(handles.pi.hProcess, INFINITE);
}