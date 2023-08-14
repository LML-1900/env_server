//
// Created by lml on 23-8-10.
//

#include "OSRMService.h"

OSRMService::OSRMService(const std::string &path) {
    config.storage_config = {path};
    config.use_shared_memory = false;
    config.algorithm = EngineConfig::Algorithm::MLD;
}

std::vector<Store::Position> OSRMService::getRoutePoints(const Store::Position &startPoint, const Store::Position &endPoint) {
    const OSRM osrm{config};
    RouteParameters params;
    params.coordinates.push_back({util::FloatLongitude{startPoint.longitude}, util::FloatLatitude{startPoint.latitude}});
    params.coordinates.push_back({util::FloatLongitude{endPoint.longitude}, util::FloatLatitude{endPoint.latitude}});
    params.geometries = osrm::engine::api::RouteParameters::GeometriesType::GeoJSON;
    params.overview = osrm::engine::api::RouteParameters::OverviewType::Full;

    // Response is in JSON format
    engine::api::ResultT result = json::Object();

    // Execute routing request, this does the heavy lifting
    const auto status = osrm.Route(params, result);

    auto &json_result = result.get<json::Object>();

    std::vector<Store::Position> routePoints;

    if (status == Status::Ok)
    {
        auto &routes = json_result.values["routes"].get<json::Array>();

        // Let's just use the first route
        auto &route = routes.values.at(0).get<json::Object>();

        auto &geometry = route.values["geometry"].get<json::Object>();
        auto &points = geometry.values["coordinates"].get<json::Array>();
        int pointNum = points.values.size();
        for (int i = 0; i < pointNum; i++) {
            auto &point = points.values.at(i).get<json::Array>();
            const auto lon = point.values.at(0).get<json::Number>().value;
            const auto lat = point.values.at(1).get<json::Number>().value;
            routePoints.push_back(Store::Position(lon, lat));
        }
    }
    else if (status == Status::Error)
    {
        const auto code = json_result.values["code"].get<json::String>().value;
        const auto message = json_result.values["message"].get<json::String>().value;

        std::cout << "Code: " << code << "\n";
        std::cout << "Message: " << code << "\n";
    }
    return routePoints;
}
