#pragma once

#include "rpc/Method.h"

#include "mocca/fs/Path.h"

struct ListScenesHandler {
    static const MethodDescription& description();
    static Method::ReturnType handle(const mocca::fs::Path& path, const JsonCpp::Value& params);
};