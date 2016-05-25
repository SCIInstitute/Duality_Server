#include "handlers/Handlers.h"

#include "common/Error.h"

#include "mocca/base/CommandLineParser.h"
#include "mocca/base/Memory.h"
#include "mocca/fs/Filesystem.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/rpc/Dispatcher.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

using mocca::CommandLineParser;
using namespace mocca::net;

std::atomic<bool> exitFlag{false};

void exitHandler(int s) {
    std::cout << "Exiting..." << std::endl;
    exitFlag = true;
}

CommandLineParser::Option DataPathOption(mocca::fs::Path& path) {
    CommandLineParser::Option option;
    option.key = "--dataPath";
    option.help = "Path to scene data (default: ./)";
    option.callback = [&](const std::string& value) { path = mocca::fs::Path(value); };
    return option;
}

CommandLineParser::Option TempPathOption(mocca::fs::Path& path) {
    CommandLineParser::Option option;
    option.key = "--tempPath";
    option.help = "Path to store temporary data (default: " + mocca::fs::tempPath().toString() + ")";
    option.callback = [&](const std::string& value) { path = mocca::fs::Path(value); };
    return option;
}

CommandLineParser::Option TCPPortOption(TCPEndpoint& endpoint) {
    CommandLineParser::Option option;
    option.key = "--tcpPort";
    option.help = "Port for plain TCP connections (default: 10123)";
    option.callback = [&](const std::string& value) { endpoint = TCPEndpoint("*", value); };
    return option;
}

CommandLineParser::Option WSPortOption(WSEndpoint& endpoint) {
    CommandLineParser::Option option;
    option.key = "--wsPort";
    option.help = "Port for plain WebSocket connections (default: 8080)";
    option.callback = [&](const std::string& value) { endpoint = WSEndpoint("*", value); };
    return option;
}

int main(int argc, const char** argv) {
    signal(SIGINT, exitHandler);

    ConnectionFactorySelector::addDefaultFactories();

    TCPEndpoint tcpEndpoint("*", "10123");
    WSEndpoint wsEndpoint("*", "8080");

    mocca::fs::Path dataPath("./");
    mocca::fs::Path tempPath(mocca::fs::tempPath());

    CommandLineParser parser;
    parser.addOption(TCPPortOption(tcpEndpoint));
    parser.addOption(WSPortOption(wsEndpoint));
    parser.addOption(DataPathOption(dataPath));
    parser.addOption(TempPathOption(tempPath));

    try {
        parser.parse(argc, argv);

        std::vector<Endpoint> endpoints{tcpEndpoint, wsEndpoint};
        auto dispatcher = mocca::make_unique<Dispatcher>(endpoints);

        ListScenesHandler listScenesHandler(dataPath);
        DownloadHandler downloadHandler(dataPath);
        PythonHandler pythonHandler(dataPath, tempPath);

        dispatcher->registerMethod(
            Method(ListScenesHandler::description(), std::bind(&ListScenesHandler::handle, &listScenesHandler, std::placeholders::_1)));
        dispatcher->registerMethod(
            Method(DownloadHandler::description(), std::bind(&DownloadHandler::handle, &downloadHandler, std::placeholders::_1)));
        dispatcher->registerMethod(
            Method(PythonHandler::description(), std::bind(&PythonHandler::handle, &pythonHandler, std::placeholders::_1)));

        dispatcher->start();

        while (!exitFlag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

    } catch (const CommandLineParser::ParserError& err) {
        LERROR(err.what() << std::endl);
        std::exit(1);
    } catch (const std::exception& err) {
        LERROR(err.what() << std::endl);
        std::exit(1);
    } catch (...) {
        LERROR("Unknown exception");
        std::exit(1);
    }
}