#pragma once

#include <fstream>
#include <map>
#include "util/array.h"

#define GOOD_ENOUGH 25699

enum thing_type {
    TTPLAYER,
    TTGROUND
};

struct thing {
    thing_type type;
    union {
        struct {
            double x;
            double y;
        } player;
        struct {
            double x1;
            double y1;
            double x2;
            double y2;
        } ground;
    };
};

struct world {
    array<thing, GOOD_ENOUGH> things;
};

double pdouble(std::string const& properties, std::string const& prop) {
    for (unsigned int i = 0; i < properties.size(); ++i) {
        if (properties.size() - i < prop.size()) {
            std::cout << prop << std::endl;
            crash("PDOBULE NO FOUND");
        }
        std::string p = properties.substr(i, prop.size());
        if (p == prop) {
            unsigned int j = i + prop.size() + 1;
            unsigned int c = 0;
            for (; j < properties.size(); ++j) {
                if (properties[j] == ',') {
                    break;
                }
                c++;
            }
            unsigned int s = i + prop.size() + 1;
            std::string v = properties.substr(s, c);
            return std::stod(v);
        }
    }
    crash("PDOUBLE NO FOUND");
    return 13.37;
}

std::map<std::string, thing (*)(std::string const& properties)> thingloaders = {
    std::make_pair("player", [](std::string const& properties){
            thing p;
            p.type = TTPLAYER;
            p.player.x = pdouble(properties, "x");
            p.player.y = pdouble(properties, "y");
            return p;
        }),
    std::make_pair("ground", [](std::string const& properties){
            thing g;
            g.type = TTGROUND;
            g.ground.x1 = pdouble(properties, "x1");
            g.ground.y1 = pdouble(properties, "y1");
            g.ground.x2 = pdouble(properties, "x2");
            g.ground.y2 = pdouble(properties, "y2");
            return g;
        }),
};

void loadworld(world& w, std::string const& filename) {
    w.things.used_size = 0; // clear that fucker
    std::ifstream file(filename);
    if (!file.good()) {
        crash("MAKE FILE GOOD AGAIN    ");
    }
    std::string line;
    while (std::getline(file, line)) {
        int i = 0;
        for (; i < line.size(); ++i) {
            if (line[i] == ' ') {
                break;
            }
        }
        std::string type = line.substr(0, i);
        std::string propss = line.substr(i + 1);
        w.things.push_back(thingloaders[type](propss));
    }
}
