#include "handlers/Handlers.h"

#include "mocca/base/CommandLineParser.h"
#include "mocca/base/Memory.h"
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

    mocca::fs::Path scenePath("d:/dmc/workspace/duality/duality-server/build/scenes");

    CommandLineParser parser;
    parser.addOption(TCPPortOption(tcpEndpoint));
    parser.addOption(WSPortOption(wsEndpoint));

    try {
        parser.parse(argc, argv);
    } catch (const CommandLineParser::ParserError& err) {
        LERROR(err.what() << std::endl);
        std::exit(1);
    }

    try {
        std::vector<Endpoint> endpoints{tcpEndpoint, wsEndpoint};
        auto dispatcher = mocca::make_unique<Dispatcher>(endpoints);

        dispatcher->registerMethod(
            Method(ListScenesHandler::description(), std::bind(&ListScenesHandler::handle, scenePath, std::placeholders::_1)));

        dispatcher->start();

        while (!exitFlag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    } catch (const std::exception& err) {
        LERROR(err.what() << std::endl);
        std::exit(1);
    } catch (...) {
        LERROR("Unknown exception");
        std::exit(1);
    }
}