#include "common/SysUtil.Posix.h"

#include "common/Error.h"

#include "mocca/base/StringTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <chrono>
#include <thread>

void SysUtilImpl::execute(const mocca::fs::Path& binary, const std::vector<std::string>& args) {
    const int errorCode = 17;
    pid_t childPid = fork();
    if (childPid == 0) { // this process is child
        std::string binaryPathStr = binary.toString();
        std::unique_ptr<char*[]> cArgs(new char*[args.size()]);
        cArgs[0] = const_cast<char*>(binaryPathStr.data());
        for (size_t i = 0; i < args.size(); ++i) {
            cArgs[i + 1] = const_cast<char*>(args[i].data());
        }
        cArgs[args.size() + 1] = NULL;
        chdir(binary.directory().data());
        if (execv(binaryPathStr.data(), cArgs.get()) == -1) {
            exit(errorCode);
        }
    } else if (childPid > 0) { // this process is parent
        // wait for child to terminate
        int status;
        pid_t pid = wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == errorCode) {
            throw Error("Could not execute process (child process terminated with error)", __FILE__, __LINE__);
        }
    } else {
        throw Error("Could not execute process (fork failed)", __FILE__, __LINE__);
    }
}