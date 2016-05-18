#pragma once

#include "handlers/TemplateProcessor.h"

#include "mocca/fs/Path.h"
#include "mocca/net/rpc/Method.h"

#include <mutex>

class AbstractHandler {
public:
    virtual ~AbstractHandler() {}

    void notify(const std::vector<JsonCpp::Value> scenes);
    mocca::net::Method::ReturnType handle(const JsonCpp::Value& params);

private:
    virtual void notifyImpl(const std::vector<JsonCpp::Value>& scenes) = 0;
    virtual mocca::net::Method::ReturnType handleImpl(const JsonCpp::Value& params) = 0;

private:
    std::mutex m_mutex;
};

class ListScenesHandler : public AbstractHandler {
public:
    static const mocca::net::MethodDescription& description();

    void notifyImpl(const std::vector<JsonCpp::Value>& scenes) override;
    mocca::net::Method::ReturnType handleImpl(const JsonCpp::Value& params) override;

private:
    std::vector<JsonCpp::Value> m_scenes;
};

class DownloadHandler : public AbstractHandler {
public:
    DownloadHandler(const mocca::fs::Path basePath);

    static const mocca::net::MethodDescription& description();
    
    void notifyImpl(const std::vector<JsonCpp::Value>& scenes) override;
    mocca::net::Method::ReturnType handleImpl(const JsonCpp::Value& params) override;

private:
    mocca::fs::Path m_basePath;
    std::map<std::string, mocca::fs::Path> m_fileMap;
};

class SCIRunHandler : public AbstractHandler {
public:
    SCIRunHandler(const mocca::fs::Path& executablePath, const mocca::fs::Path& networksPath);

    static const mocca::net::MethodDescription& description();

    void notifyImpl(const std::vector<JsonCpp::Value>& scenes) override;
    mocca::net::Method::ReturnType handleImpl(const JsonCpp::Value& params) override;

private:
    mocca::fs::Path m_executablePath;
    mocca::fs::Path m_networksPath;
    std::map<std::string, TemplateProcessor> m_processors;
};