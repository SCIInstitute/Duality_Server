#include "rpc/MethodDescription.h"

#include "rpc/ValueType.h"

#include "common/Error.h"

MethodDescription::ParameterDescription::ParameterDescription(const std::string name, JsonCpp::ValueType type,
                                                              const JsonCpp::Value& defaultValue)
    : name_(name)
    , type_(type)
    , defaultValue_(defaultValue) {
    if (!defaultValue_.empty()) {
        if (type_ != defaultValue_.type()) {
            throw Error("Type of default value does not match parameter type", __FILE__, __LINE__);
        }
    }
}

std::string MethodDescription::ParameterDescription::name() const {
    return name_;
}

JsonCpp::ValueType MethodDescription::ParameterDescription::type() const {
    return type_;
}

JsonCpp::Value MethodDescription::ParameterDescription::defaultValue() const {
    return defaultValue_;
}

std::string MethodDescription::ParameterDescription::toString() const {
    std::stringstream stream;
    stream << "parameter: " << name_ << " (" << rpc::valueTypeMapper().getByFirst(type_);
    stream << "; default: ";
    if (!defaultValue_.empty()) {
        stream << defaultValue_;
    } else {
        stream << "<None>";
    }
    stream << ")";
    return stream.str();
}

bool MethodDescription::ParameterDescription::operator<(const ParameterDescription& other) const {
    return name_ < other.name_;
}


MethodDescription::MethodDescription(const std::string& name, const std::vector<ParameterDescription>& parameters)
    : name_(name)
    , parameters_(parameters) {}

std::string MethodDescription::name() const {
    return name_;
}

std::vector<MethodDescription::ParameterDescription> MethodDescription::parameters() const {
    return parameters_;
}

std::string MethodDescription::toString() const {
    std::stringstream stream;
    stream << "method: " << name_ << '\n';
    for (const auto& param : parameters_) {
        stream << '\t' << param << '\n';
    }
    return stream.str();
}

JsonCpp::Value MethodDescription::toJson(const MethodDescription& description) {
    JsonCpp::Value result;
    result["name"] = description.name();
    auto parameters = description.parameters();
    int count = 0;
    for (const auto& param : parameters) {
        result["parameters"][count]["name"] = param.name();
        result["parameters"][count]["type"] = rpc::valueTypeMapper().getByFirst(param.type());
        if (!param.defaultValue().empty()) {
            result["parameters"][count]["default"] = param.defaultValue();
        }
        ++count;
    }
    return result;
}

MethodDescription MethodDescription::fromJson(const JsonCpp::Value& json) {
    auto methodName = json["name"].asString();
    std::vector<MethodDescription::ParameterDescription> parameters;
    auto paramsRoot = json["parameters"];
    for (auto it = paramsRoot.begin(); it != paramsRoot.end(); ++it) {
        auto paramName = (*it)["name"].asString();
        auto type = rpc::valueTypeMapper().getBySecond((*it)["type"].asString());
        JsonCpp::Value defaultValue;
        if (it->isMember("default")) {
            defaultValue = (*it)["default"];
        }
        parameters.push_back({paramName, type, defaultValue});
    }
    return MethodDescription(methodName, parameters);
}

std::ostream& operator<<(std::ostream& os, const MethodDescription::ParameterDescription& obj) {
    return os << obj.toString();
}

std::ostream& operator<<(std::ostream& os, const MethodDescription& obj) {
    return os << obj.toString();
}