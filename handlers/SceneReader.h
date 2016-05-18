#pragma once

#include "handlers/Handlers.h"

#include "mocca/fs/Path.h"
#include "mocca/base/Thread.h"

#include "jsoncpp/json.h"

#include <vector>

class SceneReader : public mocca::Runnable {
public:
    SceneReader(const mocca::fs::Path& scenePath);
    ~SceneReader();

    void update();
    void registerObserver(AbstractHandler* observer);

private:
    void run() override;

private:
    mocca::fs::Path m_scenePath;
    std::vector<AbstractHandler*> m_observers;
};