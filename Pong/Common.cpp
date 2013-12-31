//
//  Common.cpp
//  Pong
//
//  Created by Vincent K on 12/31/13.
//  Copyright (c) 2013 Unlocked Doors. All rights reserved.
//

#include "Common.h"
#include <string>


SDL_Window *mainWindow;
SDL_Renderer *renderer;

bool going = true;

const std::string ORG{"UnlockedDoors"};
const std::string GAME{"JpnGame"};

std::string PREF_PATH;
std::string BASE_PATH;

const float FPS = 60;
const float TICK_RATE = (1.0 / FPS);
const float FRAME_MS = (1000 / FPS);
const float HALF_FRAME_MS = (FRAME_MS / 2);
const float QUARTER_FRAME_MS = (FRAME_MS / 4);
const float EIGHTH_FRAME_MS = (FRAME_MS / 8);