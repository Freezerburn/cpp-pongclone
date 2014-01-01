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
    ud::Texture *toRender;
    glm::vec2 position;
    glm::vec2 size;
    
    glm::vec2 prevVelocity;
    glm::vec2 velocity;
    
    void render() {
        SDL_Rect dst;
        dst.x = static_cast<int>(glm::round(position.x));
        dst.y = static_cast<int>(glm::round(position.y));
        dst.w = static_cast<int>(glm::round(size.x));
        dst.h = static_cast<int>(glm::round(size.y));
        SDL_RenderCopy(renderer, toRender->get(), NULL, &dst);
    }
};

static const double PLAYER_VELOCITY = 150;

ud::Texture *paddleTex;
ud::Texture *ballTex;
TTF_Font *gameFont;

Entity *player;
Entity *computer;
Entity *ball;
Entity *leftWall, *rightWall;
Entity *scorePlayer, *scoreComputer;

SDL_Rect entityToRect(Entity *e) {
    SDL_Rect rect{
        static_cast<int>(glm::round(e->position.x)),
        static_cast<int>(glm::round(e->position.y)),
        static_cast<int>(glm::round(e->size.x)),
        static_cast<int>(glm::round(e->size.y))
    };
    return rect;
}

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
    
    gameFont = TTF_OpenFont("Arial.ttf", 52);
    if(!gameFont) {
        std::cout << "Could not open font: " << TTF_GetError() << std::endl;
        exit(1);
    }
    
    SDL_Color fontFg;
    fontFg.r = 255;
    fontFg.g = 255;
    fontFg.b = 255;
    fontFg.a = 255;
    
    int w, h;
    scorePlayer = new Entity();
    scorePlayer->position.x = SCORE_SIZE / 10.0;
    scorePlayer->position.y = SIMULATE_HEIGHT - (SCORE_SIZE + SCORE_SIZE / 10.0);
    TTF_SizeText(gameFont, "0", &w, &h);
    scorePlayer->size.x = w;
    scorePlayer->size.y = h;
    scorePlayer->toRender = new ud::Texture(std::make_shared<ud::Surface>(TTF_RenderText_Blended(gameFont, "0", fontFg)));

    SDL_Surface *paddleSurf = SDL_CreateRGBSurface(0, PADDLE_WIDTH, PADDLE_HEIGHT, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(paddleSurf, NULL, 0xffffffff);
    paddleTex = new ud::Texture(std::make_shared<ud::Surface>(paddleSurf));
    
    // TODO: Generate "prettier" round ball rather than square.
    SDL_Surface *ballSurf = SDL_CreateRGBSurface(0, BALL_SIZE, BALL_SIZE, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(ballSurf, NULL, 0xffffffff);
    ballTex = new ud::Texture(std::make_shared<ud::Surface>(ballSurf));
    
    player = new Entity();
    player->toRender = paddleTex;
    player->position.x = SIMULATE_WIDTH / 2.0 - PADDLE_WIDTH / 2.0;
    player->position.y = SIMULATE_HEIGHT - (PADDLE_HEIGHT + PADDLE_HEIGHT + PADDLE_HEIGHT);
    player->size.x = PADDLE_WIDTH;
    player->size.y = PADDLE_HEIGHT;
    
    computer = new Entity();
    computer->toRender = paddleTex;
    computer->position.x = SIMULATE_WIDTH / 2.0 - PADDLE_WIDTH / 2.0;
    computer->position.y = PADDLE_HEIGHT + PADDLE_HEIGHT;
    computer->size.x = PADDLE_WIDTH;
    computer->size.y = PADDLE_HEIGHT;
    
    ball = new Entity();
    ball->toRender = ballTex;
    ball->position.x = SIMULATE_WIDTH / 2.0 - BALL_SIZE / 2.0;
    ball->position.y = SIMULATE_HEIGHT / 2.0 - BALL_SIZE / 2.0;
    ball->size.x = BALL_SIZE;
    ball->size.y = BALL_SIZE;
    ball->velocity.x = 175.0;
    ball->velocity.y = 75.0;
    
    leftWall = new Entity();
    leftWall->toRender = ballTex;
    leftWall->position.x = SIMULATE_WIDTH / 7.0;
    leftWall->size.x = SIMULATE_WIDTH / 60.0;
    leftWall->size.y = SIMULATE_HEIGHT;
    
    rightWall = new Entity();
    rightWall->toRender = ballTex;
    rightWall->position.x = SIMULATE_WIDTH - SIMULATE_WIDTH / 7.0;
    rightWall->size.x = SIMULATE_WIDTH / 60.0;
    rightWall->size.y = SIMULATE_HEIGHT;
}

void checkQuit(SDL_Event *e) {
    if(e->type == SDL_KEYDOWN) {
        SDL_KeyboardEvent ke = e->key;
        if(ke.keysym.scancode == SDL_SCANCODE_Q) {
            going = false;
        }
    }
    else if(e->type == SDL_WINDOWEVENT) {
        SDL_WindowEvent we = e->window;
        if(we.type == SDL_WINDOWEVENT_CLOSE) {
            going = false;
        }
    }
}

void handleEventPlayer(SDL_Event *e) {
    static bool canHandleInput = true;
    
    if(canHandleInput) {
        if(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
            double velChange = e->type == SDL_KEYDOWN ? 1 : -1;
            SDL_KeyboardEvent ke = e->key;
            if(!ke.repeat) {
                if(ke.keysym.scancode == SDL_SCANCODE_LEFT) {
                    player->velocity.x += velChange * -PLAYER_VELOCITY;
                }
                else if(ke.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    player->velocity.x += velChange * PLAYER_VELOCITY;
                }
            }
        }
        
    }
    
    if(e->type == SDL_WINDOWEVENT) {
        SDL_WindowEvent we = e->window;
        if(we.type == SDL_WINDOWEVENT_FOCUS_LOST) {
            canHandleInput = false;
            player->prevVelocity = player->velocity;
            player->velocity.x = 0;
        }
        if(we.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
            canHandleInput = true;
            player->velocity = player->prevVelocity;
        }
    }
}

void tickPlayer() {
    player->position.x += player->velocity.x * TICK_RATE;
    player->position.y += player->velocity.y * TICK_RATE;
}

void tickComputer() {
    computer->position.x += computer->velocity.x * TICK_RATE;
    computer->position.y += computer->velocity.y * TICK_RATE;
}

void tickBall() {
    ball->position.x += ball->velocity.x * TICK_RATE;
    ball->position.y += ball->velocity.y * TICK_RATE;
    
    SDL_Rect ballRect = entityToRect(ball);
    SDL_Rect leftWallRect = entityToRect(leftWall);
    SDL_Rect rightWallRect = entityToRect(rightWall);
    SDL_Rect playerRect = entityToRect(player);
    SDL_Rect computerRect = entityToRect(computer);
    SDL_Rect result;
    
    if(SDL_IntersectRect(&ballRect, &leftWallRect, &result)) {
//        std::cout << "left" << std::endl;
//        std::cout << "x: " << result.x << std::endl;
//        std::cout << "y: " << result.y << std::endl;
//        std::cout << "w: " << result.w << std::endl;
//        std::cout << "h: " << result.h << std::endl << std::endl;
        ball->velocity.x = -ball->velocity.x;
    }
    else if(SDL_IntersectRect(&ballRect, &rightWallRect, &result)) {
//        std::cout << "right" << std::endl;
//        std::cout << "x: " << result.x << std::endl;
//        std::cout << "y: " << result.y << std::endl;
//        std::cout << "w: " << result.w << std::endl;
//        std::cout << "h: " << result.h << std::endl << std::endl;
        ball->velocity.x = -ball->velocity.x;
    }
    else if(SDL_IntersectRect(&ballRect, &playerRect, &result)) {
        ball->velocity.y = -ball->velocity.y;
    }
    else if(SDL_IntersectRect(&ballRect, &computerRect, &result)) {
        ball->velocity.y = -ball->velocity.y;
    }
    else if(ball->position.y < 0) {
        ball->position.x = SIMULATE_WIDTH / 2.0 - BALL_SIZE / 2.0;
        ball->position.y = SIMULATE_HEIGHT / 2.0 - BALL_SIZE / 2.0;
    }
    else if(ball->position.y - ball->size.y > SIMULATE_HEIGHT) {
        ball->position.x = SIMULATE_WIDTH / 2.0 - BALL_SIZE / 2.0;
        ball->position.y = SIMULATE_HEIGHT / 2.0 - BALL_SIZE / 2.0;
    }
}

void gameloop() {
    SDL_Event e;
    Uint32 cur_time = SDL_GetTicks();
    float delta = 0.0f;
    while(going) {
        Uint32 start = SDL_GetTicks();
        while(SDL_PollEvent(&e)) {
            handleEventPlayer(&e);
            checkQuit(&e);
        }
        
        Uint32 next_time = SDL_GetTicks();
        delta += (next_time - cur_time) / 1000.0f;
        cur_time = next_time;
        while(delta > TICK_RATE) {
            delta -= TICK_RATE;
            tickPlayer();
            tickComputer();
            tickBall();
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        player->render();
        computer->render();
        ball->render();
        leftWall->render();
        rightWall->render();
        scorePlayer->render();
//        scoreComputer->render();
        SDL_RenderPresent(renderer);
        
        Uint32 frame_delta = SDL_GetTicks() - start;
        int32_t sleep_time = FRAME_MS - frame_delta;
        if(sleep_time > 0) {
            SDL_Delay(sleep_time);
        }
    }
}

int main(int argc, char **argv) {
    initSDL();
    initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
    initResources();
    
    gameloop();
    
    return 0;
}