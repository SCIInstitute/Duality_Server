#include "rpc/ValueType.h"

const rpc::ValueTypeMapper& rpc::valueTypeMapper() {
    static ValueTypeMapper mapper;
    if (mapper.empty()) {
        mapper.insert(JsonCpp::ValueType::intValue, "int");
        mapper.insert(JsonCpp::ValueType::realValue, "double");
        mapper.insert(JsonCpp::ValueType::stringValue, "string");
        mapper.insert(JsonCpp::ValueType::booleanValue, "bool");
    }
    return mapper;
}