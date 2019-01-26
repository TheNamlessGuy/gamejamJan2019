#pragma once

#include <fstream>
#include <map>
#include "util/array.h"
#include "chipmunk/chipmunk.h"

#define GOOD_ENOUGH 12532

struct body {
    cpShape* shape;
    cpBody* body;
};

struct world {
    cpSpace* space = nullptr;

    array<cpShape*, GOOD_ENOUGH> grounds;
    array<body, GOOD_ENOUGH> players;

    void destroy() {
        if (!space) {
            return;
        }
        for (auto& ground : grounds) {
            cpShapeFree(ground);
        }
        for (auto& player : players) {
            cpShapeFree(player.shape);
            cpBodyFree(player.body);
        }
        grounds.used_size = 0;
        players.used_size = 0;
        cpSpaceFree(space);
    }
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

std::map<std::string, void (*)(std::string const& properties, world& w)> thingloaders = {
    std::make_pair("player", [](std::string const& properties, world& w){
            body b;
            cpFloat radius = 6;
            cpFloat mass = 3;
            cpVect pos = cpv(pdouble(properties, "x"), pdouble(properties, "y"));
            cpFloat moment = cpMomentForCircle(mass, 0, radius, cpv(0,0));
            b.body = cpSpaceAddBody(w.space, cpBodyNew(mass, moment));
            cpBodySetPosition(b.body, pos);
            b.shape = cpSpaceAddShape(w.space, cpCircleShapeNew(b.body, radius, cpv(0,0)));
            cpShapeSetFriction(b.shape, 0.7);
            w.players.push_back(b);
        }),
    std::make_pair("ground", [](std::string const& properties, world& w){
            cpVect start = cpv(pdouble(properties, "x1"), pdouble(properties, "y1"));
            cpVect end = cpv(pdouble(properties, "x2"), pdouble(properties, "y2"));
            cpShape* s = cpSegmentShapeNew(cpSpaceGetStaticBody(w.space), start, end, 5);
            cpShapeSetFriction(s, 1);
            cpSpaceAddShape(w.space, s);
            w.grounds.push_back(s);
        }),
};

void loadworld(world& w, std::string const& filename) {
    w.destroy();
    w.space = cpSpaceNew();
    cpSpaceSetGravity(w.space, cpv(0, 200));
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
        thingloaders[type](propss, w);
    }
}

void destroytheworld(world& w) {
    w.destroy();
}

void updatew(world& w) {
    cpSpaceStep(w.space, 1.0f / MAXIMUM_PERCIEVABLE_FRAMERATE);
}
