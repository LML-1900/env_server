//
// Created by lml on 23-7-27.
//

#ifndef IMPORT_DEM_DEM_H
#define IMPORT_DEM_DEM_H
#include <string>
#include <vector>
namespace Store {
    struct Position {
        double longitude;
        double latitude;

        Position(double lon, double lat) : longitude(lon), latitude(lat) {}
    };

    struct Crater {
        Position position;
        double width;
        double depth;

        Crater(Position pos, double w, double d) : position(pos), width(w), depth(d) {}

        Crater() : position(0, 0), width(0), depth(0) {}
    };

    struct DemData {
        std::vector<Crater> craters;
        std::string content;
    };
}


#endif //IMPORT_DEM_DEM_H