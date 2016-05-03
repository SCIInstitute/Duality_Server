#pragma once

#include "jsoncpp/json.h"

#include <string>
#include <vector>

class MethodDescription {
public:
    class ParameterDescription {
    public:
        ParameterDescription(const std::string name, JsonCpp::ValueType type, const JsonCpp::Value& defaultValue);

        std::string name() const;
        JsonCpp::ValueType type() const;
        JsonCpp::Value defaultValue() const;

        std::string toString() const;

        bool operator<(const ParameterDescription& other) const;

    private:
        std::string name_;
        JsonCpp::ValueType type_;
        JsonCpp::Value defaultValue_;
    };

    static JsonCpp::Value toJson(const MethodDescription& description);
    static MethodDescription fromJson(const JsonCpp::Value& json);

    MethodDescription(const std::string& name, const std::vector<ParameterDescription>& parameters);

    std::string name() const;
    std::vector<ParameterDescription> parameters() const;

    std::string toString() const;

private:
    std::string name_;
    std::vector<ParameterDescription> parameters_;
};

std::ostream& operator<<(std::ostream& os, const MethodDescription::ParameterDescription& obj);
std::ostream& operator<<(std::ostream& os, const MethodDescription& obj);