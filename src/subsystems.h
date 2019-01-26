#pragma once

#include "frameworks.h"
#include "engine_data.h"
#include "events.h"

void init_subsystems()
{
    auto& window    = engine_data->sdl2_data.window;
    auto& renderer  = engine_data->sdl2_data.renderer;
    
    window.handle   = SDL_CreateWindow( window.title,
                                       window.pos.x, window.pos.y,
                                       window.size.x, window.size.y,
                                       window.flags );
    
    renderer.handle = SDL_CreateRenderer( window.handle, -1, renderer.flags );
    
    init_events();
    
    //SDL_PumpEvents();
}

void close_subsystems()
{
    // close_events();
    
    auto& window    = engine_data->sdl2_data.window;
    auto& renderer  = engine_data->sdl2_data.renderer;
    
    SDL_DestroyRenderer( renderer.handle );
    SDL_DestroyWindow( window.handle );
}

template < void (*const draw)() >
void render()
{
    auto& renderer = engine_data->sdl2_data.renderer;
    
    if ( SDL_RenderTargetSupported( renderer.handle ) ) {
        SDL_SetRenderTarget( renderer.handle, nullptr );
    }
    SDL_SetRenderDrawColor( renderer.handle, 0, 0, 0, 255 );
    SDL_RenderClear( renderer.handle );
    
    if ( renderer.recalc_size ) {
        auto& window = engine_data->sdl2_data.window;
        
        SDL_GetWindowSize( window.handle, &window.size.x, &window.size.y );
        
        renderer.recalc_size = false;
    }
    
    draw();
    
    SDL_RenderPresent( renderer.handle );
}

void handle_sys_events()
{
    // system events are handled in event filter instead
    SDL_PumpEvents();
}

void time_sync()
{
    SDL_Delay( 1000 / engine_data->sdl2_data.renderer.target_frame_rate );
}
