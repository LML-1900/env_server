//
// Created by lml on 23-8-6.
//

#ifndef ENV_SERVER_ENVIRONMENTSERVICE_H
#define ENV_SERVER_ENVIRONMENTSERVICE_H
#include <vector>
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <google/protobuf/repeated_field.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "../store/MySQLClient.h"
#include "../store/MongoClient.h"
#include "../store/Dem.h"
#include "env_service.grpc.pb.h"
#include "env_service.pb.h"
#include "OSRMService.h"


using std::vector;
using std::string;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using environmentdata::EnvironmentData;
using environmentdata::Area;
using environmentdata::GetDataRequest;
using environmentdata::GetDataResponse;
using environmentdata::CraterArea;
using environmentdata::DataType;
using environmentdata::Crater;
using environmentdata::Position;
using environmentdata::StartStopPoints;
using environmentdata::RoutePoints;

class EnvironmentService final : public EnvironmentData::Service{
public:
    grpc::Status GetData(ServerContext* context, const GetDataRequest* dataRequest, ServerWriter<GetDataResponse>* writer) override;

    grpc::Status UpdateCrater(ServerContext* context, const Crater* crater, CraterArea* response) override;

    grpc::Status GetRoutePoints(ServerContext* context, const StartStopPoints* startStopPoints, environmentdata::RoutePoints *response) override;

    EnvironmentService();

    ~EnvironmentService();
private:
    const int HIGHESTLEVEL = 14;

    OSRMService *osrmService;

    void searchData(string tileID, int fileType, environmentdata::GetDataResponse *response);

    vector<string> LonLatToTileID(const Position bottomLeft, const Position topRight, int level);

    Position makePosition(double longitude, double latitude);
};


#endif //ENV_SERVER_ENVIRONMENTSERVICE_H
