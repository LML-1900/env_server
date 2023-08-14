#include <iostream>
#include <string>
#include "store/ImportData.h"
#include "service/EnvironmentService.h"
#include "store/Dem.h"
#include "service/OSRMService.h"

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
//    OSRMService osrmService("/home/lml/OSRM/map/zhuhai/zhuhai.osrm");
//    Store::Position start(113.5439372, 22.2180642);
//    Store::Position end(113.5425177, 22.2252363);
//    std::vector<Store::Position> routePoints = osrmService.getRoutePoints(start, end);
//    for (const auto &point: routePoints) {
//        std::cout << point.longitude << " " << point.latitude << std::endl;
//    }
//    return 0;
}
//
//int main() {
//    std::string dirPath = "/home/lml/env_server/11-736-158demData/11_736_158_WGS84_terrain";
//    ImportData::getInstance()->readDemDirAndImportToDB(dirPath);
//}
