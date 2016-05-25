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

class PythonHandler {
public:
    PythonHandler(const mocca::fs::Path& basePath, const mocca::fs::Path& outputPath);
    static const mocca::net::MethodDescription& description();
    mocca::net::Method::ReturnType handle(const JsonCpp::Value& params);

private:
    mocca::fs::Path m_basePath;
    mocca::fs::Path m_outputPath;
};