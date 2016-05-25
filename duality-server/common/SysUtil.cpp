#include "common/SysUtil.h"

#include "common/Error.h"
#if defined WIN32
#include "common/SysUtil.Windows.h"
#else
#include "common/SysUtil.Posix.h"
#endif

#include "mocca/fs/Filesystem.h"


TempFile::TempFile(const mocca::fs::Path& path, const std::string& text)
    : path_(path) {
    if (mocca::fs::exists(path_)) {
        mocca::fs::removeFile(path_);
    }
    file_.open(path_);
    file_ << text;
    file_.close();
}

TempFile::~TempFile() {
    if (file_.is_open()) {
        file_.close();
    }
   // mocca::fs::removeFile(path_);
}


void SysUtil::execute(const mocca::fs::Path& binary, const std::vector<std::string> args) {
    return SysUtilImpl::execute(binary, args);
}
