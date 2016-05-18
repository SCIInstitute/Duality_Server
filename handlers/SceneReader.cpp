#include "handlers/SceneReader.h"

#include "common/Error.h"

#include "mocca/fs/Filesystem.h"

#include <fstream>

SceneReader::SceneReader(const mocca::fs::Path& scenePath)
    : m_scenePath(scenePath) {}

SceneReader::~SceneReader() {
    join();
}

void SceneReader::update() {
    auto files = mocca::fs::directoryContents(m_scenePath);
    std::vector<JsonCpp::Value> scenes;
    for (const auto& file : files) {
        if (file.extension() == ".json") {
            JsonCpp::Reader reader;
            std::ifstream stream(file.toString().data());
            JsonCpp::Value scene;
            if (!reader.parse(stream, scene)) {
                throw Error("Error parsing file " + file.toString() + ": " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
            }
            scenes.push_back(scene);
        }
    }

    for (auto observer : m_observers) {
        observer->notify(scenes);
    }
}

void SceneReader::registerObserver(AbstractHandler* observer) {
    m_observers.push_back(observer);
}

void SceneReader::run() {
    while (!isInterrupted()) {
        update();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
