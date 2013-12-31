//
//  Common.h
//  Pong
//
//  Created by Vincent K on 12/31/13.
//  Copyright (c) 2013 Unlocked Doors. All rights reserved.
//

#ifndef Pong_Common_h
#define Pong_Common_h

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SIMULATE_WIDTH = 640;
const int SIMULATE_HEIGHT = 480;
const float GLOBAL_SCALE_WIDTH = WINDOW_WIDTH / SIMULATE_WIDTH;
const float GLOBAL_SCALE_HEIGHT = WINDOW_HEIGHT / SIMULATE_HEIGHT;

const double PADDLE_WIDTH = SIMULATE_WIDTH / 15.0;
const double PADDLE_HEIGHT = PADDLE_WIDTH / 10.0;
const double BALL_SIZE = PADDLE_HEIGHT;

const Uint32 WINDOW_FLAGS = SDL_WINDOW_OPENGL |
    SDL_WINDOW_ALLOW_HIGHDPI |
    SDL_WINDOW_SHOWN |
    SDL_WINDOW_MOUSE_FOCUS |
    SDL_WINDOW_INPUT_FOCUS;

extern SDL_Window *mainWindow;
extern SDL_Renderer *renderer;

#endif
