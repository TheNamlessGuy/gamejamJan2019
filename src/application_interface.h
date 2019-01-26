#pragma once

#define DUMMY {}

#include <iostream>

SDL_Texture* example = nullptr;
SDL_Rect examplerect = {32, 32, 32, 32};

void drawt(SDL_Texture* t, SDL_Rect* r) {
    SDL_RenderCopy(engine_data->sdl2_data.renderer.handle, t, nullptr, r);
}

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

void update_application_logic() {

}

void draw_application_view() {
    drawt(example, &examplerect);
}

void init_application() {
    example = load_or_die("res/image/example.jpg");
}

void close_application() {
    SDL_DestroyTexture(example);
}
