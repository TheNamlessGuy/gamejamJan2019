#pragma once

#include <fstream>
#include <map>
#include <cmath>
#include "util/array.h"
#include "chipmunk/chipmunk.h"

#define GOOD_ENOUGH 12532
#define CTPLAYER 0
#define CTGROUND 1

struct player {
    cpShape* shape;
    cpBody* body;
    bool standing;
};

struct world {
  cpSpace* space = nullptr;

    array<cpShape*, GOOD_ENOUGH> grounds;
    array<player, GOOD_ENOUGH> players;

    cpCollisionHandler* pgch;

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
    std::make_pair("ground", [](std::string const& properties, world& w){
            cpVect start = cpv(pdouble(properties, "x1"), pdouble(properties, "y1"));
            cpVect end = cpv(pdouble(properties, "x2"), pdouble(properties, "y2"));
            cpShape* s = cpSegmentShapeNew(cpSpaceGetStaticBody(w.space), start, end, 1);
            cpShapeSetCollisionType(s, CTGROUND);
            cpShapeSetFriction(s, 1);
            cpSpaceAddShape(w.space, s);
            w.grounds.push_back(s);
        }),
};

void loadworld(world& w, std::string const& filename) {
    w.destroy();
    w.space = cpSpaceNew();
    w.pgch = cpSpaceAddCollisionHandler(w.space, CTPLAYER, CTGROUND);
    w.pgch->beginFunc = collisionstart;
    w.pgch->separateFunc = collisionend;
    w.pgch->userData = (void*)&w;
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
        std::string type = line.substr(0, i);
        std::string propss = line.substr(i + 1);
        thingloaders[type](propss, w);
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
      const cpVect& vec = cpBodyGetVelocity(player.body);
      cpVect impulse = cpv(0, 0);
      if (vec.x > -100) impulse.x -= 100;
      cpBodyApplyImpulseAtLocalPoint(player.body, cpvrotate(impulse, cpBodyGetRotation(player.body)), cpv(0, 0));
    }
  } else if (RIGHT) {
    for (auto& player : w.players) {
      const cpVect& vec = cpBodyGetVelocity(player.body);
      cpVect impulse = cpv(0, 0);
      if (vec.x < 100) impulse.x += 100;
      cpBodyApplyImpulseAtLocalPoint(player.body, cpvrotate(impulse, cpBodyGetRotation(player.body)), cpv(0, 0));
    }
  }
  cpSpaceStep(w.space, 1.0f / MAXIMUM_PERCIEVABLE_FRAMERATE);
}

void drawworld_debug(world& w) {
  for (auto& player : w.players) {
    cpVect pos = cpBodyGetPosition(player.body);
    cpFloat radius = cpPolyShapeGetRadius(player.shape);
    cpVect rot = cpBodyGetRotation(player.body);
    double t = atan2(rot.y, rot.x);
    for (int i = 1; i <= cpPolyShapeGetCount(player.shape); ++i) {
      cpVect from = cpPolyShapeGetVert(player.shape, i - 1);
      int j = i;
      if (j == cpPolyShapeGetCount(player.shape)) {
        j = 0;
      }
      cpVect to = cpPolyShapeGetVert(player.shape, j);
      double fromt = atan2(from.y, from.x);
      double fromr = sqrt(from.y*from.y + from.x*from.x);
      double tot = atan2(to.y, to.x);
      double tor = sqrt(to.y*to.y + to.x*to.x);

      double fromx = fromr * cos(t + fromt);
      double fromy = fromr * sin(t + fromt);
      double tox = tor * cos(t + tot);
      double toy = tor * sin(t + tot);

      thickLineRGBA(engine_data->sdl2_data.renderer.handle,
                    pos.x + fromx,
                    pos.y + fromy,
                    pos.x + tox,
                    pos.y + toy,
                    radius,
                    255,
                    255,
                    255,
                    255);
    }
  }
  for (auto& ground : w.grounds) {
    cpVect a = cpSegmentShapeGetA(ground);
    cpVect b = cpSegmentShapeGetB(ground);
    thickLineRGBA(engine_data->sdl2_data.renderer.handle,
                  a.x,
                  a.y,
                  b.x,
                  b.y,
                  1,
                  0,
                  255,
                  0,
                  255);
  }
}
