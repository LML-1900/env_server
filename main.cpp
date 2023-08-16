#include <iostream>
#include <string>
#include "store/ImportData.h"
#include "service/EnvironmentService.h"
#include "store/Dem.h"

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    EnvironmentService service;

//    grpc::EnableDefaultHealthCheckService(true);
//    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
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
    RunServer();
    return 0;
}
//
//int main() {
//    std::string dirPath = "/home/lml/env_server/11-736-158demData/11_736_158_WGS84_terrain";
//    ImportData::getInstance()->readDemDirAndImportToDB(dirPath);
//}
