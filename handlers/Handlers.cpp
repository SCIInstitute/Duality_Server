#include "handlers/Handlers.h"

#include "mocca/fs/Filesystem.h"

#include <fstream>

using namespace mocca::net;

const MethodDescription& ListScenesHandler::description() {
    static MethodDescription description("ListScenes", {});
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
