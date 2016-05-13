#include "handlers/Handlers.h"

#include "mocca/fs/Filesystem.h"
#include "mocca/log/LogManager.h"

#include <fstream>

using namespace mocca::net;

const MethodDescription& ListScenesHandler::description() {
    static MethodDescription description("listScenes", {});
    return description;
}

Method::ReturnType ListScenesHandler::handle(const mocca::fs::Path& path, const JsonCpp::Value& params) {
    Method::ReturnType result;
    auto files = mocca::fs::directoryContents(path);
    int i = 0;
    for (const auto& file : files) {
        if (file.extension() == ".json") {
            JsonCpp::Reader reader;
            std::ifstream stream(file.toString().data());
            reader.parse(stream, result.first[i++]);
        }
    }
    return result;
}

const MethodDescription& DownloadHandler::description() {
    static MethodDescription::ParameterDescription pathParam("path", JsonCpp::ValueType::stringValue, JsonCpp::Value());
    static MethodDescription description("download", {pathParam});
    return description;
}

Method::ReturnType DownloadHandler::handle(const mocca::fs::Path& basePath, const JsonCpp::Value& params) {
    mocca::fs::Path path = basePath + params["path"].asString();
    auto binary = mocca::net::MessagePart(mocca::fs::readBinaryFile(path));
    JsonCpp::Value root;
    root["type"] = "g3d"; // FIXME
    return std::make_pair(root, std::vector<mocca::net::MessagePart>{binary});
}

const mocca::net::MethodDescription& SCIRunHandler::description() {
    static MethodDescription::ParameterDescription pathParam("params", JsonCpp::ValueType::objectValue, JsonCpp::Value());
    static MethodDescription description("scirun", {pathParam});
    return description;
}

mocca::net::Method::ReturnType SCIRunHandler::handle(const JsonCpp::Value& params) {
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