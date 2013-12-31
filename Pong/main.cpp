//
//  main.cpp
//  Pong
//
//  Created by Vincent K on 12/31/13.
//  Copyright (c) 2013 Unlocked Doors. All rights reserved.
//

#include <iostream>
#include <vector>

#include "glm.hpp"

#include "Common.h"
#include "Surface.h"
#include "Texture.h"


class Entity {
public:
    std::vector<ud::Texture*> toRender;
    glm::vec2 position;
    glm::vec2 size;
    
    void render() {
    }
};

ud::Texture *paddleTex;
ud::Texture *ballTex;
TTF_Font *gameFont;

Entity *player;
Entity *computer;
Entity *ball;
Entity *score;

void initSDL() {
    int err = SDL_Init(SDL_INIT_EVERYTHING);
    if(err) {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        exit(err);
    }
    
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    err = IMG_Init(flags);
    if((err & flags) != flags) {
        std::cout << "Could not initalize SDL_Image: " << IMG_GetError() << std::endl;
        exit(err);
    }
    
    err = TTF_Init();
    if(err) {
        std::cout << "Coult not initialize SDL_ttf: " << TTF_GetError() << std::endl;
    }
}

void initWindow(int width, int height, Uint32 flags) {
    int err = SDL_CreateWindowAndRenderer(width, height, flags, &mainWindow, &renderer);
    if(err) {
        std::cout << "Could not create window and renderer: " << SDL_GetError() << std::endl;
        exit(err);
    }
}

void initResources() {
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    
    gameFont = TTF_OpenFont("Arial", 12);

    SDL_Surface *paddleSurf = SDL_CreateRGBSurface(0, PADDLE_WIDTH, PADDLE_HEIGHT, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(paddleSurf, NULL, 0);
    paddleTex = new ud::Texture(std::make_shared<ud::Surface>(paddleSurf));
    
    // TODO: Generate "prettier" round ball rather than square.
    SDL_Surface *ballSurf = SDL_CreateRGBSurface(0, BALL_SIZE, BALL_SIZE, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(ballSurf, NULL, 0);
    ballTex = new ud::Texture(std::make_shared<ud::Surface>(ballSurf));
    
    player = new Entity();
    player->toRender.push_back(paddleTex);
    player->position.x = SIMULATE_WIDTH / 2.0 - PADDLE_WIDTH / 2.0;
    player->position.y = PADDLE_HEIGHT + PADDLE_HEIGHT / 5.0;
    player->size.x = PADDLE_WIDTH;
    player->size.y = PADDLE_HEIGHT;
    
    computer = new Entity();
    computer->toRender.push_back(paddleTex);
    computer->position.x = SIMULATE_WIDTH / 2.0 - PADDLE_WIDTH / 2.0;
    computer->position.y = PADDLE_HEIGHT + PADDLE_HEIGHT / 5.0;
    computer->size.x = PADDLE_WIDTH;
    computer->size.y = PADDLE_HEIGHT;
    
    ball = new Entity();
    ball->toRender.push_back(ballTex);
    ball->position.x = SIMULATE_WIDTH / 2.0 - BALL_SIZE / 2.0;
    ball->position.y = SIMULATE_HEIGHT / 2.0 - BALL_SIZE / 2.0;
    ball->size.x = BALL_SIZE;
    ball->size.y = BALL_SIZE;
    
    score = new Entity();
}

int main(int argc, char **argv) {
    initSDL();
    initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
    
    return 0;
}