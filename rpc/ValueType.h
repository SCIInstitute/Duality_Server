#pragma once

#include "mocca/base/BidirectionalMap.h"

#include "jsoncpp/json.h"

namespace rpc {
    using ValueTypeMapper = mocca::BidirectionalMap<JsonCpp::ValueType, std::string>;
    const ValueTypeMapper& valueTypeMapper();
}