#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

// VS2013 doesn't support the 'noexcept' directive
#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#define CODE_LOCATION_IN_ERROR

class Error : public std::runtime_error {
public:
    Error(const std::string& msg, const std::string& file, int line)
        : std::runtime_error("")
        , msg_(msg)
        , file_(file)
        , line_(line) {
        std::stringstream str;
        str << msg_;
#ifdef CODE_LOCATION_IN_ERROR
        if (file_.empty() && line_ == -1)
            str << " (" << file_ << ":" << std::to_string(line_) << ")";
#endif
        msg_ = str.str();
    }

    virtual ~Error() {}

    const char* what() const NOEXCEPT override { return msg_.c_str(); }

private:
    std::string msg_;
    std::string file_;
    int line_;
};