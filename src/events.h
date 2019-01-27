#pragma once

#include "frameworks.h"
#include "engine_data.h"

void handle_window_event( SDL_WindowEvent* e );
void handle_key_press( SDL_KeyboardEvent* e );
void handle_mouse_motion( SDL_MouseMotionEvent* e );
void handle_mouse_button_press( SDL_MouseButtonEvent* e );
void handle_mouse_wheel_motion( SDL_MouseWheelEvent* e );

int SDL2_event_filter_function( void*, SDL_Event* );

void init_events()
{
    SDL_SetEventFilter( SDL2_event_filter_function, nullptr );

    // events that are ignored:
    SDL_EventState( SDL_TEXTEDITING,        SDL_DISABLE );
    SDL_EventState( SDL_TEXTINPUT,          SDL_DISABLE );
    SDL_EventState( SDL_KEYMAPCHANGED,      SDL_DISABLE );

    SDL_EventState( SDL_CLIPBOARDUPDATE,    SDL_DISABLE );
    SDL_EventState( SDL_DROPFILE,           SDL_DISABLE );
    // SDL_EventState( SDL_DROPTEXT,           SDL_DISABLE );
    // SDL_EventState( SDL_DROPBEGIN,          SDL_DISABLE );
    // SDL_EventState( SDL_DROPCOMPLETE,       SDL_DISABLE );
}

int SDL2_event_filter_function( void*, SDL_Event* e )
{
    switch ( e->type ) {
        case SDL_QUIT:
            engine_data->quit_requested = true;
            break;

        case SDL_WINDOWEVENT:
            handle_window_event( &( e->window ) );
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            handle_key_press( &( e->key ) );
            break;

        case SDL_MOUSEMOTION:
            handle_mouse_motion( &( e->motion ) );
            break;

        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            handle_mouse_button_press( &( e->button ) );
            break;

        case SDL_MOUSEWHEEL:
            handle_mouse_wheel_motion( &( e->wheel ) );
            break;

        case SDL_APP_LOWMEMORY:         crash( "OS is out of memory." );                break;
        case SDL_APP_TERMINATING:       crash( "'" PROGRAM_NAME "' was terminated." );  break;
        case SDL_RENDER_DEVICE_RESET:   crash( "render device was reset." );            break;
        case SDL_RENDER_TARGETS_RESET:  crash( "render targets were reset." );          break;

        default: return 1; // unhandled events can be passed on for now...
    }

    return 0; // no events are ever passed to the SDL event queue
}

void handle_window_event( SDL_WindowEvent* e )
{
    switch ( e->event )
    {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_EXPOSED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        case SDL_WINDOWEVENT_MAXIMIZED:
            engine_data->sdl2_data.renderer.recalc_size = true;
            break;

        case SDL_WINDOWEVENT_CLOSE:
            engine_data->quit_requested = true;
            break;

        default:
            break;
    }
}

bool LEFT = false;
bool RIGHT = false;
bool UP = false;

bool RESET = false;

void handle_key_press( SDL_KeyboardEvent* e )
{
    switch (e->keysym.sym) {
        case SDLK_LEFT:
            LEFT = (e->state == SDL_PRESSED);
            break;
        case SDLK_RIGHT:
            RIGHT = (e->state == SDL_PRESSED);
            break;
        case SDLK_UP:
            UP = (e->state == SDL_PRESSED);
            break;
        case SDLK_r:
            RESET = (e->state == SDL_PRESSED);
            break;
        default:
            break;
    }
    // e->keysym.scancode or sym
    /*
      e->state;
      e->repeat;
      e->keysym.mod;
    */
}

void handle_mouse_motion( SDL_MouseMotionEvent* e )
{

}

void handle_mouse_button_press( SDL_MouseButtonEvent* e )
{
    /*
      e->clicks;
      e->state;
      e->button;
      e->x;
      e->y;
    */
}

void handle_mouse_wheel_motion( SDL_MouseWheelEvent* e )
{
    /*
      e->x;
      e->y;
      e->direction;
    */
}
