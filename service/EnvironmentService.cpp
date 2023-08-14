//
// Created by lml on 23-8-6.
//

#include "EnvironmentService.h"

Status EnvironmentService::SayHello(ServerContext* context, const HelloRequest* request,HelloReply* reply) {
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
}

Status EnvironmentService::UpdateCrater(grpc::ServerContext *context, const environmentdata::Crater *crater,
                                        environmentdata::CraterArea *response) {
    vector<string> tileIDs = LonLatToTileID(crater->pos(), crater->pos(), HIGHESTLEVEL);
    if (tileIDs.size() == 0) {
        return Status(grpc::OUT_OF_RANGE, "can't find crater's pos in database.");
    }
    int id = MySQLClient::getInstance()->getDataIdFromFileStorage(tileIDs[0], DataType::DEM);
    vector<Store::Crater> craters = {Store::Crater(Store::Position(crater->pos().longitude(), crater->pos().latitude()), crater->width(), crater->depth())};
    MongoClient::getInstance()->insertCraters(id, craters);
    return Status::OK;
}

Status EnvironmentService::GetData(grpc::ServerContext *context, const GetDataRequest* dataRequest, ServerWriter<GetDataResponse>* writer) {
    auto tileIDs = LonLatToTileID(dataRequest->area().bottomleft(), dataRequest->area().topright(), dataRequest->level());
    for (const auto &tileID: tileIDs)
    {
        GetDataResponse re;
        searchData(tileID, dataRequest->datatype(), &re);
        writer->Write(re);
    }
    return Status::OK;
}

void EnvironmentService::searchData(string tileID, int fileType, environmentdata::GetDataResponse *response) {
    int id = MySQLClient::getInstance()->getDataIdFromFileStorage(tileID, fileType);
    Store::DemData data = MongoClient::getInstance()->getDemDataById(id);
    response->set_content(data.content);
    response->set_tileid(tileID);
    for (const auto &i: data.craters) {
        Crater* crater = response->add_craters();
        crater->set_depth(i.depth);
        crater->set_width(i.width);
        crater->mutable_pos()->CopyFrom(makePosition(i.position.longitude, i.position.latitude));
    }
}

Position EnvironmentService::makePosition(double longitude, double latitude) {
    Position pos;
    pos.set_longitude(longitude);
    pos.set_latitude(latitude);
    return pos;
}

std::vector<std::string> EnvironmentService::LonLatToTileID(const Position bottomLeft, const Position topRight, int level){
    float minlon = bottomLeft.longitude();
    float minlat = bottomLeft.latitude();
    float maxlon = topRight.longitude();
    float maxlat = topRight.latitude();
    int n = pow(2, level + 1);
    float Xmin, Ymin, Xmax, Ymax;
    Xmin = (minlon + 180) / 360 * n;
    Ymin = (minlat + 90) / 360 * n;
    Xmax = (maxlon + 180) / 360 * n;
    Ymax = (maxlat + 90) / 360 * n;
    int xmin, ymin, xmax, ymax;
    xmin = floor(Xmin); ymin = floor(Ymin);
    xmax = floor(Xmax); ymax = floor(Ymax);
    string Level = std::to_string(level);
    vector<string> ans;
    int i = xmin, j = ymin;
    while (i <= xmax)
    {
        while (j <= ymax)
        {
            ans.push_back(Level + "_" + std::to_string(i) + "_" + std::to_string(j));
            j++;
        }
        i++;
        j = ymin;
    }
    return ans;
}