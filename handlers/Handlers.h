#pragma once

#include "handlers/TemplateProcessor.h"

#include "mocca/fs/Path.h"
#include "mocca/net/rpc/Method.h"

class ListScenesHandler {
public:
    ListScenesHandler(const mocca::fs::Path& basePath);
    static const mocca::net::MethodDescription& description();
    mocca::net::Method::ReturnType handle(const JsonCpp::Value& params);
private:
    mocca::fs::Path m_basePath;
};

class DownloadHandler {
public:
    DownloadHandler(const mocca::fs::Path& basePath);
    static const mocca::net::MethodDescription& description();
    mocca::net::Method::ReturnType handle(const JsonCpp::Value& params);
private:
    mocca::fs::Path m_basePath;
};

class SCIRunHandler {
public:
    SCIRunHandler(const mocca::fs::Path& executablePath, const mocca::fs::Path& networksPath);
    static const mocca::net::MethodDescription& description();
    mocca::net::Method::ReturnType handle(const JsonCpp::Value& params);

private:
    mocca::fs::Path m_executablePath;
    mocca::fs::Path m_basePath;
};