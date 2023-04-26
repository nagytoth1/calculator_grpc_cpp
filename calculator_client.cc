/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpcpp/grpcpp.h>
#include "cmake/build/calculator.grpc.pb.h"

ABSL_FLAG(std::string, target, "localhost:6000", "Server address");

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using calculator::CalculatorService;
using calculator::MathOperands;
using calculator::MathResult;

class CalculatorClient {
public:
    CalculatorClient(std::shared_ptr<Channel> channel)
            : serverStub(CalculatorService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string add(const int& a, const int& b) {
        // Data we are sending to the server.
        calculator::MathOperands request;
        request.set_operand1(a);
        request.set_operand2(b);

        // Container for the data we expect from the server.
        MathResult result;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = serverStub->Add(&context, request, &result);

        // Act upon its status.
        if (status.ok()) {
            return std::to_string(result.result());
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }
    //ezt át kéne alakítani, hogy csak a függvényhívás változzon
    std::string sub(const int& a, const int& b) {
        // Data we are sending to the server.
        calculator::MathOperands request;
        request.set_operand1(a);
        request.set_operand2(b);

        // Container for the data we expect from the server.
        MathResult result;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = serverStub->Sub(&context, request, &result);

        // Act upon its status.
        if (status.ok()) {
            return std::to_string(result.result());
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }
private:
    std::unique_ptr<CalculatorService::Stub> serverStub;
};

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    std::string target_str = absl::GetFlag(FLAGS_target);
    // We indicate that the channel isn't authenticated (use of
    // InsecureChannelCredentials()).
    CalculatorClient calculator(
            grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    int a, b;
    std::cout << "a = ";
    std::cin >> a;
    std::cout << "b = ";
    std::cin >> b;

    std::cout << "ADD - Calculator result: " << calculator.add(a, b) << std::endl;
    std::cout << "SUB - Calculator result: " << calculator.sub(a, b) << std::endl;
    /*
    std::cout << "MUL - Calculator result: " << calculator.mul(&a, &b) << std::endl;
    std::cout << "DIV - Calculator result: " << calculator.div(&a, &b) << std::endl;
     */
    return 0;
}
