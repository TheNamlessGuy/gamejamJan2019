#pragma once

#include <iostream>
#include "chipmunk/chipmunk.h"
#include "world.h"

SDL_Texture* example = nullptr;

// World stuff
world w;
cpSpace* space = nullptr;
cpVect gravity = cpv(0, -100);

// World stuff stuff
cpShape* ground = nullptr;
cpBody* ballBody = nullptr;
cpShape* ballShape = nullptr;

void drawt(SDL_Texture* t, SDL_Rect* r) {
    SDL_RenderCopy(engine_data->sdl2_data.renderer.handle, t, nullptr, r);
}

#include "database.h"

struct ImageLoader {
    using type = SDL_Texture;

    static type* load( const std::string& s ) {
        SDL_Texture* t = IMG_LoadTexture( engine_data->sdl2_data.renderer.handle, s.c_str() );
        if ( t == nullptr ) {
            std::string msg = std::string( "OMGFUCKYOU CANNOT LOAD THOSE FINE TAPESTRIES: " ) + s;
            crash( msg.c_str() ); // crash kill and destroy
        }
        return t;
    }

    static void unload( type* t ) {
        SDL_DestroyTexture(t);
    }

    static uint64_t size() { return 0; } // unused for now...
};

database<ImageLoader> imdb;

SDL_Texture* load_or_die(const char* imagefilename) {
    return imdb.add( std::string( imagefilename ) );
}

void update_application_logic() {
    cpVect pos = cpBodyGetPosition(ballBody);
    cpVect vel = cpBodyGetVelocity(ballBody);
    std::cout << "POS: " << pos.x << " " << pos.y << std::endl;
    std::cout << "VEL: " << vel.x << " " << vel.y << std::endl;
    std::cout << std::endl;
    cpSpaceStep(space, 1.0f / 24.0f);
}

void draw_application_view() {
    for (auto& thing : w.things) {
        switch (thing.type) {
            case TTPLAYER: {
                SDL_Rect f;
                f.x = thing.player.x;
                f.y = thing.player.y;
                f.w = 32;
                f.h = 32;
                drawt(example, &f);
                break;
            }
            case TTGROUND: {
                lineRGBA(engine_data->sdl2_data.renderer.handle,
                         thing.ground.x1,
                         thing.ground.y1,
                         thing.ground.x2,
                         thing.ground.y2,
                         255,
                         0,
                         0,
                         255);
                break;
            }
            default: {
                crash("I AINT DRAWING SHIT");
            }
        }
    }
}

void init_application() {
    example = load_or_die("res/image/example.jpg");

    // literal copy-pasta from docs https://chipmunk-physics.net/release/ChipmunkLatest-Docs/
    space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);
    ground = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(-20, 5), cpv(20, -5), 0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(space, ground);




    cpFloat radius = 5;
    cpFloat mass = 1;

    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpv(0,0));

    ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPosition(ballBody, cpv(0, 15));

    ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpv(0,0)));
    cpShapeSetFriction(ballShape, 0.7);

    loadworld(w, "res/maps/example.csv");
}

void close_application() {
    cpShapeFree(ballShape);
    cpBodyFree(ballBody);
    cpShapeFree(ground);
    cpSpaceFree(space);
    SDL_DestroyTexture(example);
}
