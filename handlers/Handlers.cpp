#include "handlers/Handlers.h"

#include "common/SysUtil.h"

#include "mocca/fs/Filesystem.h"
#include "mocca/log/LogManager.h"

#include <cassert>

using namespace mocca::net;

void AbstractHandler::notify(const std::vector<JsonCpp::Value> scenes) {
    std::lock_guard<std::mutex> lock(m_mutex);
    notifyImpl(scenes);
}

mocca::net::Method::ReturnType AbstractHandler::handle(const JsonCpp::Value& params) {
    std::lock_guard<std::mutex> lock(m_mutex);
    return handleImpl(params);
}

const MethodDescription& ListScenesHandler::description() {
    static MethodDescription description("listScenes", {});
    return description;
}

void ListScenesHandler::notifyImpl(const std::vector<JsonCpp::Value>& scenes) {
    m_scenes = scenes;
}

Method::ReturnType ListScenesHandler::handleImpl(const JsonCpp::Value& params) {
    Method::ReturnType result;
    for (const auto& scene : m_scenes) {
        result.first.append(scene);
    }
    return result;
}

DownloadHandler::DownloadHandler(const mocca::fs::Path basePath)
    : m_basePath(basePath) {}

const MethodDescription& DownloadHandler::description() {
    static MethodDescription::ParameterDescription pathParam("path", JsonCpp::ValueType::stringValue, JsonCpp::Value());
    static MethodDescription description("download", {pathParam});
    return description;
}

void DownloadHandler::notifyImpl(const std::vector<JsonCpp::Value>& scenes) {
    for (const auto& scene : scenes) { // FIXME: too much code duplication
        const auto& sceneNode = scene["scene"];
        for (const auto& objectNode : sceneNode) {
            const auto& sourceNode = objectNode["source"];
            if (sourceNode["type"] != "download") {
                continue;
            }
            auto name = objectNode["name"].asString();
            auto filename = sourceNode["filename"].asString();
            mocca::fs::Path path = m_basePath + name + filename;
            if (!mocca::fs::exists(path)) {
                LERROR("Referenced file '" << path.toString() << "' does not exist");
            }
            m_fileMap[name] = path;
        }
    }
}

Method::ReturnType DownloadHandler::handleImpl(const JsonCpp::Value& params) {
    mocca::fs::Path path = m_fileMap[params["name"].asString()];
    auto binary = mocca::net::MessagePart(mocca::fs::readBinaryFile(path));
    JsonCpp::Value root;
    root["type"] = "g3d"; // FIXME
    return std::make_pair(root, std::vector<mocca::net::MessagePart>{binary});
}

SCIRunHandler::SCIRunHandler(const mocca::fs::Path& executablePath, const mocca::fs::Path& networksPath)
    : m_executablePath(executablePath)
    , m_networksPath(networksPath) {}

const mocca::net::MethodDescription& SCIRunHandler::description() {
    static MethodDescription::ParameterDescription pathParam("params", JsonCpp::ValueType::objectValue, JsonCpp::Value());
    static MethodDescription description("scirun", {pathParam});
    return description;
}

void SCIRunHandler::notifyImpl(const std::vector<JsonCpp::Value>& scenes) {
    for (const auto& scene : scenes) {
        const auto& sceneNode = scene["scene"];
        for (const auto& objectNode : sceneNode) {
            const auto& sourceNode = objectNode["source"];
            if (sourceNode["type"] != "scirun") {
                continue;
            }
            auto name = objectNode["name"].asString();
            auto filename = sourceNode["filename"].asString();
            mocca::fs::Path path = m_networksPath + name + filename;
            if (!mocca::fs::exists(path)) {
                LERROR("Referenced file '" << path.toString() << "' does not exist");
            }

            std::string templ = mocca::fs::readTextFile(path);
            m_processors.insert(std::make_pair(name, TemplateProcessor(templ, sourceNode["parameters"])));
        }
    }
}

mocca::net::Method::ReturnType SCIRunHandler::handleImpl(const JsonCpp::Value& params) {
    auto it = m_processors.find(params["name"].asString());
    assert(it != end(m_processors));
    const auto& processor = it->second;
    std::string script = processor.createScript(params["values"]);
    TempFile tmpFile(mocca::fs::Path("temp.py"), script);
    SysUtil::execute(m_executablePath, {"-s temp.py", "--headless", "--no_splash"});


    /* FIXME: JUST A DUMMY IMPLEMENTATION */
    int intVal = static_cast<int>(params["params"]["val"].asFloat());
    bool even = (intVal % 2 == 0);
    mocca::fs::Path path = even ? mocca::fs::Path("file1.g3d") : mocca::fs::Path("file2.g3d");
    LINFO("Sending file " << path.toString());
    auto binary = mocca::net::MessagePart(mocca::fs::readBinaryFile(path));
    JsonCpp::Value root;
    root["type"] = "g3d";
    return std::make_pair(root, std::vector<mocca::net::MessagePart>{binary});
}
