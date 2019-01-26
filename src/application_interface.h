#pragma once

#define DUMMY {}

#include <iostream>
#include "chipmunk/chipmunk.h"

SDL_Texture* example = nullptr;
SDL_Rect examplerect = {32, 32, 32, 32};

// World stuff
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

/*
SDL_Texture* load_or_die(const char* imagefilename) {
    SDL_Surface* surf = IMG_Load(imagefilename);
    char msg[1025]; // big enough
    if (!surf) {
        sprintf(msg, "OMGFUCKYOU CANNOT LOAD THOSE FINE TAPESTRIES: %s", imagefilename);
        crash(msg); // crash kill and destroy
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(engine_data->sdl2_data.renderer.handle, surf);
    SDL_FreeSurface(surf);
    if (!tex) {
        sprintf(msg, "NO TEXTURES NO LIFE: %s", imagefilename);
        crash(msg);
    }
    return tex;
}
*/

void update_application_logic() {
    cpVect pos = cpBodyGetPosition(ballBody);
    cpVect vel = cpBodyGetVelocity(ballBody);
    std::cout << "POS: " << pos.x << " " << pos.y << std::endl;
    std::cout << "VEL: " << vel.x << " " << vel.y << std::endl;
    std::cout << std::endl;
    cpSpaceStep(space, 1.0f / 24.0f);
}

void draw_application_view() {
    drawt(example, &examplerect);
    lineRGBA(engine_data->sdl2_data.renderer.handle, 128, 128, 365, 256, 255, 0, 0, 255);
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
}

void close_application() {
    cpShapeFree(ballShape);
    cpBodyFree(ballBody);
    cpShapeFree(ground);
    cpSpaceFree(space);
    SDL_DestroyTexture(example);
}
