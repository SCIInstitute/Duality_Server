#pragma once

#include "jsoncpp/json.h"

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

class TemplateProcessor {
public:
    static std::string createScript(const std::string& templ, const JsonCpp::Value& params);
private:
    struct VariableIndex {
        std::ptrdiff_t position;
        std::string name;

        bool operator<(const VariableIndex& other) const;
    };
};