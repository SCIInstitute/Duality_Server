#include "handlers/Handlers.h"

#include "mocca/fs/Filesystem.h"

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
    mocca::net::Message binary;
    mocca::fs::Path path = basePath + params["path"].asString();
    std::ifstream file(path.toString().data(), std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        int size = static_cast<int>(file.tellg());
        auto buffer = std::make_shared<std::vector<uint8_t>>(size);
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(buffer->data()), size);
        file.close();
        binary.push_back(buffer);
    }
    JsonCpp::Value root;
    root["type"] = "g3d";
    return std::make_pair(root, binary);
}
