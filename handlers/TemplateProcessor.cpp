#include "handlers/TemplateProcessor.h"

#include "common/Error.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"

#include <regex>

bool TemplateProcessor::VariableIndex::operator<(const VariableIndex& other) const {
    return position < other.position;
}

TemplateProcessor::TemplateProcessor(const std::string& templ, const JsonCpp::Value& parameters)
    : m_templ(templ), m_parameters(parameters) {
    buildIndex();
}

void TemplateProcessor::buildIndex() {
    for (const auto& param : m_parameters) {
        std::string name = param["name"].asString();
        std::regex regexVar("\\W(" + name + ")\\W");
        auto varIt = std::sregex_iterator(begin(m_templ), end(m_templ), regexVar);
        for (; varIt != std::sregex_iterator(); ++varIt) {
            VariableIndex index{varIt->position(1), name};
            m_indices.push_back(index);
        }
    }
    std::sort(begin(m_indices), end(m_indices));
}

std::string TemplateProcessor::createScript(const JsonCpp::Value& args) const {
    std::stringstream result;

    // replace variables in the script with the corresponding values from the JSON
    size_t previous = 0;
    for (size_t i = 0; i < m_indices.size(); ++i) {
        const auto& index = m_indices[i];
        const auto& name = index.name;

        // append part from end of previous match to beginning of current match
        result << m_templ.substr(previous, index.position - previous);

        // append value
        JsonCpp::Value value = args[name];
        std::string valueString = value.toStyledString();
        mocca::replaceAll(valueString, "\n", ""); // workaround, because toStyledString inserts linebreak
        result << valueString;

        // remember end of current match for next iteration
        previous = index.position + name.size();
    }
    // append part from end of last match to end of template
    result << m_templ.substr(previous, std::string::npos);
    return result.str();
}