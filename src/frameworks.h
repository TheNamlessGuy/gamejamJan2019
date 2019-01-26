#pragma once

#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL2_gfxPrimitives.h>

void init_frameworks()
{
    SDL_SetHint( SDL_HINT_THREAD_STACK_SIZE, "65536" ); // 64KB stacks
    SDL_Init( SDL_INIT_EVERYTHING );
    SDLNet_Init();
    IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG );
    TTF_Init();
    Mix_Init( MIX_INIT_MP3 );
}

void close_frameworks()
{
    Mix_Quit();
    TTF_Quit();
    SDLNet_Quit();
    IMG_Quit();
    SDL_Quit();
}

void crash( const char* msg = "-" )
{
    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR,
                             "'" PROGRAM_NAME "' crashed due to an error",
                             msg, nullptr );
    exit( -1 );
}
