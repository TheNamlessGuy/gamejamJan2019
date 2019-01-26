#pragma once

#include <iostream>
#include "chipmunk/chipmunk.h"
#include "world.h"

SDL_Texture* example = nullptr;

// World stuff
world w;

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
    updatew(w);
}

void draw_application_view() {
    drawworld_debug(w);
}

void init_application() {
    example = load_or_die("res/image/example.jpg");
    loadworld(w, "res/maps/example.csv");
}

void close_application() {
    destroytheworld(w);
    SDL_DestroyTexture(example);
}
