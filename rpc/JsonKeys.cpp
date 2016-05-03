#include "rpc/JsonKeys.h"

const std::string& rpc::methodKey() {
    static std::string key = "method";
    return key;
}

const std::string& rpc::paramsKey() {
    static std::string key = "params";
    return key;
}

const std::string& rpc::statusKey() {
    static std::string key = "status";
    return key;
}

const std::string& rpc::errorKey() {
    static std::string key = "error";
    return key;
}

const std::string& rpc::resultKey() {
    static std::string key = "result";
    return key;
}

const std::string& rpc::successStatus() {
    static std::string key = "success";
    return key;
}

const std::string& rpc::errorStatus() {
    static std::string key = "error";
    return key;
}

const std::string& rpc::describe() {
    static std::string key = "rpc::describe";
    return key;
}