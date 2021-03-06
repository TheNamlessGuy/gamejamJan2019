#pragma once

#include <fstream>
#include <map>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "util/array.h"
#include "chipmunk/chipmunk.h"

#define GOOD_ENOUGH 12532
#define CTPLAYER 0
#define CTGROUND 1
#define CTHOUSE 2
#define CTANNOYING_FRIEND 3
#define CTMEL0N 4

struct player {
    cpShape* shape;
    cpBody* body;
    bool standing;
};

struct house {
    cpShape* shape;
    cpBody* body;
};

struct annoying_friend {
    cpShape* shape;
    cpBody* body;
    unsigned int jumpcounter;
    unsigned int anothercounter;
};

struct melon {
    cpShape* shape;
    cpBody* body;
};

struct world {
    cpSpace* space = nullptr;

    array<cpShape*, GOOD_ENOUGH> grounds;
    array<player, GOOD_ENOUGH> players;
    array<house, GOOD_ENOUGH> houses;
    array<annoying_friend, GOOD_ENOUGH> annoying_friends;
    array<melon, GOOD_ENOUGH> melons;

    cpCollisionHandler* pgch;
    cpCollisionHandler* pach;
    cpCollisionHandler* pmch;
    cpCollisionHandler* mhch;
    cpCollisionHandler* ahch;

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
        for (auto& house : houses) {
            cpShapeFree(house.shape);
            cpBodyFree(house.body);
        }
        for (auto& annoying_friend : annoying_friends) {
            cpShapeFree(annoying_friend.shape);
            cpBodyFree(annoying_friend.body);
        }
        for (auto& melon : melons) {
            cpShapeFree(melon.shape);
            cpBodyFree(melon.body);
        }
        grounds.used_size = 0;
        players.used_size = 0;
        houses.used_size = 0;
        annoying_friends.used_size = 0;
        melons.used_size = 0;
        cpSpaceFree(space);
    }
};

unsigned char collisionstart(cpArbiter *arb, struct cpSpace *space, void* data) {
    world* w = (world*)data;
    cpShape* player;
    cpShape* ground;
    cpArbiterGetShapes(arb, &player, &ground);
    for (auto& p : w->players) {
        if (p.shape == player) {
            p.standing = true;
            break;
        }
    }
    return 1;
}

void collisionend(cpArbiter *arb, struct cpSpace *space, cpDataPointer data) {
    world* w = (world*)data;
    cpShape* player;
    cpShape* ground;
    cpArbiterGetShapes(arb, &player, &ground);
    for (auto& p : w->players) {
        if (p.shape == player) {
            p.standing = false;
            break;
        }
    }
}

unsigned char collisionstart2(cpArbiter *arb, struct cpSpace *space, void* data) {
    crash("U LOOOOOSE");
    return 1;
}

unsigned char collisionstart3(cpArbiter *arb, struct cpSpace *space, void* data) {
    crash("U ARE WINNER");
    return 1;
}

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
            player b;
            b.standing = false;
            cpFloat radius = 12;
            cpFloat mass = 3;
            cpVect pos = cpv(pdouble(properties, "x"), pdouble(properties, "y"));
            cpFloat moment = cpMomentForCircle(mass, 0, radius, cpv(0,0));
            b.body = cpSpaceAddBody(w.space, cpBodyNew(mass, moment));
            cpBodySetPosition(b.body, pos);
            b.shape = cpBoxShapeNew(b.body, 32, 32, 1);
            cpShapeSetCollisionType(b.shape, CTPLAYER);
            cpShapeSetFriction(b.shape, 0.7);
            cpSpaceAddShape(w.space, b.shape);
            w.players.push_back(b);
        }),
    std::make_pair("house", [](std::string const& properties, world& w){
            house b;
            cpFloat radius = 12;
            cpFloat mass = 3;
            cpVect pos = cpv(pdouble(properties, "x"), pdouble(properties, "y"));
            cpFloat moment = cpMomentForCircle(mass, 0, radius, cpv(0,0));
            b.body = cpSpaceAddBody(w.space, cpBodyNew(mass, moment));
            cpBodySetPosition(b.body, pos);
            cpVect vects[5] = {cpv(-70,70),cpv(70,70),cpv(70,-70),cpv(0,-140),cpv(-70,-70)};
            b.shape = cpPolyShapeNewRaw(b.body, 5, vects, 1);
            cpShapeSetCollisionType(b.shape, CTHOUSE);
            cpShapeSetFriction(b.shape, 0.15);
            cpSpaceAddShape(w.space, b.shape);
            w.houses.push_back(b);
        }),
    std::make_pair("ground", [](std::string const& properties, world& w){
            cpVect start = cpv(pdouble(properties, "x1"), pdouble(properties, "y1"));
            cpVect end = cpv(pdouble(properties, "x2"), pdouble(properties, "y2"));
            cpShape* s = cpSegmentShapeNew(cpSpaceGetStaticBody(w.space), start, end, 1);
            cpShapeSetCollisionType(s, CTGROUND);
            cpShapeSetFriction(s, 1);
            cpSpaceAddShape(w.space, s);
            w.grounds.push_back(s);
        }),
    std::make_pair("annoying_friend", [](std::string const& properties, world& w){
            annoying_friend b;
            b.jumpcounter = (rand() % 250) + 1;
            b.anothercounter = (rand() % 130) + 1;
            cpFloat radius = 20;
            cpFloat mass = 1;
            cpVect pos = cpv(pdouble(properties, "x"), pdouble(properties, "y"));
            cpFloat moment = cpMomentForCircle(mass, 0, radius, cpv(0,0));
            b.body = cpSpaceAddBody(w.space, cpBodyNew(mass, moment));
            cpBodySetPosition(b.body, pos);
            b.shape = cpSpaceAddShape(w.space, cpCircleShapeNew(b.body, radius, cpv(0,0)));
            cpShapeSetCollisionType(b.shape, CTANNOYING_FRIEND);
            cpShapeSetFriction(b.shape, 0.9);
            w.annoying_friends.push_back(b);
        }),
    std::make_pair("melon", [](std::string const& properties, world& w){
            melon b;
            cpFloat radius = 20;
            cpFloat mass = 1;
            cpVect pos = cpv(pdouble(properties, "x"), pdouble(properties, "y"));
            cpFloat moment = cpMomentForCircle(mass, 0, radius, cpv(0,0));
            b.body = cpSpaceAddBody(w.space, cpBodyNew(mass, moment));
            cpBodySetPosition(b.body, pos);
            b.shape = cpSpaceAddShape(w.space, cpCircleShapeNew(b.body, radius, cpv(0,0)));
            cpShapeSetCollisionType(b.shape, CTMEL0N);
            cpShapeSetFriction(b.shape, 0.9);
            w.melons.push_back(b);
        }),
};

void loadworld(world& w, std::string const& filename) {
    srand(time(NULL));

    w.destroy();
    w.space = cpSpaceNew();

    w.pgch = cpSpaceAddCollisionHandler(w.space, CTPLAYER, CTGROUND);
    w.pgch->beginFunc = collisionstart;
    w.pgch->separateFunc = collisionend;
    w.pgch->userData = (void*)&w;

    w.pach = cpSpaceAddCollisionHandler(w.space, CTPLAYER, CTANNOYING_FRIEND);
    w.pach->beginFunc = collisionstart;
    w.pach->separateFunc = collisionend;
    w.pach->userData = (void*)&w;

    w.pmch = cpSpaceAddCollisionHandler(w.space, CTPLAYER, CTMEL0N);
    w.pmch->beginFunc = collisionstart;
    w.pmch->separateFunc = collisionend;
    w.pmch->userData = (void*)&w;

    w.mhch = cpSpaceAddCollisionHandler(w.space, CTMEL0N, CTHOUSE);
    w.mhch->beginFunc = collisionstart3;
    w.mhch->userData = (void*)&w;

    w.ahch = cpSpaceAddCollisionHandler(w.space, CTANNOYING_FRIEND, CTHOUSE);
    w.ahch->beginFunc = collisionstart2;
    w.ahch->userData = (void*)&w;

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

#include "events.h"

void updatew(world& w) {
    if (LEFT) {
        for (auto& player : w.players) {
            if (player.standing) {
                const cpVect& vec = cpBodyGetVelocity(player.body);
                cpVect impulse = cpv(0, 0);
                impulse.x -= 100;
                impulse.y = vec.y;
                cpBodySetVelocity(player.body, impulse);
            }
        }
    } else if (RIGHT) {
        for (auto& player : w.players) {
            if (player.standing) {
                const cpVect& vec = cpBodyGetVelocity(player.body);
                cpVect impulse = cpv(0, 0);
                impulse.x += 100;
                impulse.y = vec.y;
                cpBodySetVelocity(player.body, impulse);
            }
        }
    }
    if (UP) {
        for (auto& player : w.players) {
            if (player.standing) {
                const cpVect& vec = cpBodyGetVelocity(player.body);
                cpVect impulse = cpv(0, 0);
                impulse.x = vec.x;
                impulse.y -= 200;
                cpBodySetVelocity(player.body, impulse);
            }
        }
    }
    if (RESET) {
        RESET = false;
        for (auto& player : w.players) {
            cpVect pos = cpBodyGetPosition(player.body);
            pos.y = 0;
            cpBodySetPosition(player.body, pos);
            cpBodySetVelocity(player.body, cpv(0, 0));
        }
    }
    if (DASH && (LEFT || RIGHT)) {
        DASH = false;
        for (auto& player : w.players) {
            if (player.standing) {
                const cpVect& vec = cpBodyGetVelocity(player.body);
                cpVect impulse = cpv(0, 0);
                if (LEFT) {
                    impulse.x = vec.x - 250;
                    impulse.y = vec.y - 65;
                } else {
                    impulse.x = vec.x + 250;
                    impulse.y = vec.y - 65;
                }
                cpBodySetVelocity(player.body, impulse);
            }
        }
    }
    for (auto& annoying_friend : w.annoying_friends) {
        annoying_friend.jumpcounter--;
        annoying_friend.anothercounter--;
        if (!annoying_friend.jumpcounter) {
            annoying_friend.jumpcounter = (rand() % 230) + 1;
            const cpVect& vec = cpBodyGetVelocity(annoying_friend.body);
            cpVect impulse = cpv(0, 0);
            impulse.x = vec.x;
            impulse.y = vec.y - 150;
            cpBodySetVelocity(annoying_friend.body, impulse);
        }
        if (!annoying_friend.anothercounter) {
            melon& m = w.melons.data[0];
            cpVect melonpos = cpBodyGetPosition(m.body);
            cpVect friendpos = cpBodyGetPosition(annoying_friend.body);
            annoying_friend.anothercounter = (rand() % 130) + 1;
            const cpVect& vec = cpBodyGetVelocity(annoying_friend.body);
            cpVect impulse = cpv(0, 0);
            unsigned int d = rand() % 2;
            bool go_closer = (rand() % 3) < 2;
            if (d || (go_closer && melonpos.x < friendpos.x)) {
                impulse.x = vec.x - 120;
                impulse.y = vec.y - 30;
            }
            if (!d || (go_closer && melonpos.x >= friendpos.y)) {
                impulse.x = vec.x + 120;
                impulse.y = vec.y - 30;
            }
            cpBodySetVelocity(annoying_friend.body, impulse);
        }
    }
    cpSpaceStep(w.space, 1.0f / MAXIMUM_PERCIEVABLE_FRAMERATE);
}

void draw_vector_thing(cpBody* body, cpShape* shape, double camx, double camy) {
    cpVect pos = cpBodyGetPosition(body);
    cpFloat radius = cpPolyShapeGetRadius(shape);
    cpVect rot = cpBodyGetRotation(body);
    double t = atan2(rot.y, rot.x);
    for (int i = 1; i <= cpPolyShapeGetCount(shape); ++i) {
        cpVect from = cpPolyShapeGetVert(shape, i - 1);
        int j = i;
        if (j == cpPolyShapeGetCount(shape)) {
            j = 0;
        }
        cpVect to = cpPolyShapeGetVert(shape, j);
        double fromt = atan2(from.y, from.x);
        double fromr = sqrt(from.y*from.y + from.x*from.x);
        double tot = atan2(to.y, to.x);
        double tor = sqrt(to.y*to.y + to.x*to.x);

        double fromx = fromr * cos(t + fromt);
        double fromy = fromr * sin(t + fromt);
        double tox = tor * cos(t + tot);
        double toy = tor * sin(t + tot);

        thickLineRGBA(engine_data->sdl2_data.renderer.handle,
                      pos.x + fromx - camx + 320,
                      pos.y + fromy - camy + 240,
                      pos.x + tox - camx + 320,
                      pos.y + toy - camy + 240,
                      radius,
                      255,
                      255,
                      255,
                      255);
    }
}

void drawworld_debug(world& w) {
    if (!w.players.used_size) {
        crash("ONE PLAYER PLZ OMG");
    }
    for (auto& player : w.players) {
        cpVect pos = cpBodyGetPosition(player.body);
        draw_vector_thing(player.body, player.shape, pos.x, pos.y);
    }
    cpVect playerpos = cpBodyGetPosition(w.players.data[0].body);
    for (auto& ground : w.grounds) {
        cpVect a = cpSegmentShapeGetA(ground);
        cpVect b = cpSegmentShapeGetB(ground);
        thickLineRGBA(engine_data->sdl2_data.renderer.handle,
                      a.x - playerpos.x + 320,
                      a.y - playerpos.y + 240,
                      b.x - playerpos.x + 320,
                      b.y - playerpos.y + 240,
                      1,
                      0,
                      255,
                      0,
                      255);
    }
}
