#pragma once

#include "jsoncpp/json.h"

#include <string>
#include <set>
#include <vector>
#include <unordered_map>

class TemplateProcessor {
public:
    TemplateProcessor(const std::string& templ, const JsonCpp::Value& parameters);

    std::string createScript(const JsonCpp::Value& params) const;

private:
    struct VariableIndex {
        std::ptrdiff_t position;
        std::string name;

        bool operator<(const VariableIndex& other) const;
    };

    void buildIndex();

private:
    std::string m_templ;
    JsonCpp::Value m_parameters;
    std::vector<VariableIndex> m_indices; // indices must be sorted by position
};