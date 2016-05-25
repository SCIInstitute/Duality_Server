#include "handlers/TemplateProcessor.h"

#include "mocca/base/StringTools.h"

#include <regex>

bool TemplateProcessor::VariableIndex::operator<(const VariableIndex& other) const {
    return position < other.position;
}

std::string TemplateProcessor::createScript(const std::string& templ, const JsonCpp::Value& args) {
    std::vector<VariableIndex> indices;
    for (auto it = args.begin(); it != args.end(); ++it) {
        std::string name = it.key().asString();
        std::regex regexVar("\\W(" + name + ")\\W");
        auto varIt = std::sregex_iterator(begin(templ), end(templ), regexVar);
        for (; varIt != std::sregex_iterator(); ++varIt) {
            auto position = varIt->position(1);
            indices.push_back({ position, name });
        }
    }
    std::sort(begin(indices), end(indices));

    // replace variables in the script with the corresponding values from the JSON
    std::stringstream result;
    size_t previous = 0;
    for (size_t i = 0; i < indices.size(); ++i) {
        const auto& index = indices[i];
        const auto& name = index.name;

        // append part from end of previous match to beginning of current match
        result << templ.substr(previous, index.position - previous);

        // append value
        JsonCpp::Value value = args[name];
        std::string valueString = value.toStyledString();
        mocca::replaceAll(valueString, "\n", ""); // workaround, because toStyledString inserts linebreak
        result << valueString;

        // remember end of current match for next iteration
        previous = index.position + name.size();
    }
    // append part from end of last match to end of template
    result << templ.substr(previous, std::string::npos);
    return result.str();
}