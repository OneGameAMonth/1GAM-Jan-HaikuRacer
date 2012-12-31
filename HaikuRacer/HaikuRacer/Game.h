//
//  Game.h
//  HaikuRacer
//
//  Created by Kyle Halladay on 12-12-31.
//  Copyright (c) 2012 Team Chilly. All rights reserved.
//

#include "BtOgreFramework.h"
#include "GameStateManager.h"

class Game
{
public:
	Game();
    ~Game();
    
	void runGame();
    
    private:
	GameStateManager *mAppStateManager;
};



