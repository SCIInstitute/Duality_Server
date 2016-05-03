#include "rpc/Method.h"

Method::Method(const MethodDescription& description, const CallbackType callback)
    : description_(description)
    , callback_(callback) {}

std::string Method::name() const {
    return description_.name();
}

MethodDescription Method::description() const {
    return description_;
}

Method::ReturnType Method::operator()(const JsonCpp::Value& params) const {
    return callback_(params);
}