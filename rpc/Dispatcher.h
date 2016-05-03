#pragma once

#include "rpc/Method.h"

#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/Endpoint.h"

#include "jsoncpp/json.h"

#include <vector>

class MoccaJsonServer : public mocca::Runnable {
public:
    MoccaJsonServer(const std::vector<mocca::net::Endpoint>& endpoints);
    ~MoccaJsonServer();

    void registerMethod(Method method);

private:
    void run() override;
    void sendReply(const JsonCpp::Value root, const std::vector<mocca::net::MessagePart>& binary, std::shared_ptr<const mocca::net::ConnectionID> connectionID);
    JsonCpp::Value parseMessage(const mocca::net::Message& message);
    void registerReflection();

private:
    std::unique_ptr<mocca::net::ConnectionAggregator> aggregator_;
    std::vector<Method> methods_;
};