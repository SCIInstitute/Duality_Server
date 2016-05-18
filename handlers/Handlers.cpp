#include "handlers/Handlers.h"

#include "common/SysUtil.h"

#include "mocca/fs/Filesystem.h"
#include "mocca/log/LogManager.h"

using namespace mocca::net;

ListScenesHandler::ListScenesHandler(const mocca::fs::Path& basePath)
    : m_basePath(basePath) {}

const MethodDescription& ListScenesHandler::description() {
    static MethodDescription description("listScenes", {});
    return description;
}

Method::ReturnType ListScenesHandler::handle(const JsonCpp::Value& params) {
    Method::ReturnType result;
    auto dirContents = mocca::fs::directoryContents(m_basePath);
    for (const auto& path : dirContents) {
        if (mocca::fs::isDirectory(path)) {
            mocca::fs::Path scenePath = path + "scene.json";
            if (!mocca::fs::exists(scenePath)) {
                LERROR("Scene definition file '" << scenePath.toString() << "' does not exist");
            } else {
                JsonCpp::Reader reader;
                JsonCpp::Value root;
                std::string jsonStr = mocca::fs::readTextFile(scenePath);
                if (!reader.parse(jsonStr, root)) {
                    LERROR("Error parsing JSON: " << reader.getFormattedErrorMessages());
                } else {
                    result.first.append(root);
                }
            }
        }
    }
    return result;
}

DownloadHandler::DownloadHandler(const mocca::fs::Path& basePath)
    : m_basePath(basePath) {}

const MethodDescription& DownloadHandler::description() {
    static MethodDescription::ParameterDescription pathParam("path", JsonCpp::ValueType::stringValue, JsonCpp::Value());
    static MethodDescription description("download", {pathParam});
    return description;
}

Method::ReturnType DownloadHandler::handle(const JsonCpp::Value& params) {
    std::string sceneName = params["scene"].asString();
    mocca::fs::Path path = m_basePath + sceneName + "download" + params["filename"].asString();
    auto binary = mocca::net::MessagePart(mocca::fs::readBinaryFile(path));
    JsonCpp::Value root;
    root["type"] = "g3d"; // FIXME
    return std::make_pair(root, std::vector<mocca::net::MessagePart>{binary});
}

SCIRunHandler::SCIRunHandler(const mocca::fs::Path& executablePath, const mocca::fs::Path& networksPath)
    : m_executablePath(executablePath)
    , m_basePath(networksPath) {}

const mocca::net::MethodDescription& SCIRunHandler::description() {
    static MethodDescription::ParameterDescription pathParam("params", JsonCpp::ValueType::objectValue, JsonCpp::Value());
    static MethodDescription description("scirun", {pathParam});
    return description;
}

mocca::net::Method::ReturnType SCIRunHandler::handle(const JsonCpp::Value& params) {
    std::string sceneName = params["scene"].asString();
    mocca::fs::Path path = m_basePath + sceneName + "SCIRun" + params["filename"].asString();
    std::string templ = mocca::fs::readTextFile(path);
    std::string script = TemplateProcessor::createScript(templ, params["values"]);
    mocca::fs::Path tmpPath("temp.py");
    TempFile tmpFile(tmpPath, script);
    SysUtil::execute(m_executablePath, {"-s " + tmpPath.toString(), "--headless", "--no_splash"});


    {
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
}
