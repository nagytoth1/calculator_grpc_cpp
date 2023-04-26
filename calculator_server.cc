#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "cmake/build/calculator.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using calculator::CalculatorService;
using calculator::MathOperands;
using calculator::MathResult;

ABSL_FLAG(uint16_t, port, 6000, "Server port for the service");

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public CalculatorService::Service {
public:
    Status Add(ServerContext *context,
               const MathOperands *request,
               MathResult *response) override {
        response->set_result(request->operand1() + request->operand2());
        return Status::OK;
    }
    Status Sub(ServerContext *context,
               const MathOperands *request,
               MathResult *response) override {
        response->set_result(request->operand1() - request->operand2());
        return Status::OK;
    }
    Status Mul(ServerContext *context,
               const MathOperands *request,
               MathResult *response) override {
        response->set_result(request->operand1() * request->operand2());
        return Status::OK;
    }
    Status Div(ServerContext *context,
               const MathOperands *request,
               MathResult *response) override {
        response->set_result(request->operand1() / request->operand2());
        return Status::OK;
    }
};

void RunServer(uint16_t port) {
    std::string server_address = absl::StrFormat("127.0.0.1:%d", port);
    GreeterServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer(absl::GetFlag(FLAGS_port));
    return 0;
}
