//
// Created by lml on 23-8-10.
//

#ifndef ENV_SERVER_OSRMSERVICE_H
#define ENV_SERVER_OSRMSERVICE_H
#include <string>
#include <vector>
#include <iostream>
#include "osrm/match_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"
#include "../store/Dem.h"
using namespace osrm;

class OSRMService {
public:
    OSRMService(const std::string &path);

    std::vector<Store::Position> getRoutePoints(const Store::Position &startPoint, const Store::Position &endPoint);

private:
    EngineConfig config;
};


#endif //ENV_SERVER_OSRMSERVICE_H
