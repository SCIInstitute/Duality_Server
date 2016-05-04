#pragma once

#include "mocca/fs/Path.h"
#include "mocca/net/rpc/Method.h"

struct ListScenesHandler {
    static const mocca::net::MethodDescription& description();
    static mocca::net::Method::ReturnType handle(const mocca::fs::Path& path, const JsonCpp::Value& params);
};