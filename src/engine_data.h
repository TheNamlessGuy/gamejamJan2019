#pragma once

#include "math.h"
#include "common.h"
#include "frameworks.h"
#include "database.h"

#include <vector>

struct
{
    bool quit_requested = false;

    struct {
        struct {
            const SDL_threadID main_thread_id = SDL_GetThreadID( nullptr );
        } threads;

        struct {
            SDL_Window*     handle      = nullptr;
            const char*     title       = PROGRAM_NAME;
            SDL_Point       pos         { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
            SDL_Point       size        { 640, 480 };
            int             flags       = SDL_WINDOW_SHOWN;
        } window;

        struct {
            SDL_Renderer*   handle              = nullptr;
            int             flags               = SDL_RENDERER_TARGETTEXTURE
                                                    | SDL_RENDERER_PRESENTVSYNC
                                                    | SDL_RENDERER_ACCELERATED;
            bool            recalc_size         = true;
            int             target_frame_rate   = 24;
        } renderer;

    } sdl2_data;

    struct {
        const int           ram_size_mb         = SDL_GetSystemRAM();
        const int           num_logical_cores   = SDL_GetCPUCount();
        const char*         os                  = SDL_GetPlatform();
    } sys_info;
} *engine_data;

void init_engine_data( int argc, const char* argv[] )
{
    // engine data is allocated on the heap:
    engine_data = new ANONYMOUS_TYPE_FROM_PTR( engine_data ){};

    // get display refresh-rate:
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(
        SDL_GetWindowDisplayIndex( engine_data->sdl2_data.window.handle ), &mode );
    // engine_data->sdl2_data.renderer.target_frame_rate = mode.refresh_rate;

    // TODO:
    // - read config file   (optional)
    // - parse arguments    (optional)
}

void close_engine_data()
{
    delete engine_data;
}
