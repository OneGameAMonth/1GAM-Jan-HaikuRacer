//
//  Game.h
//  HaikuRacer
//
//  Created by Kyle Halladay on 12-12-31.
//  Copyright (c) 2012 Team Chilly. All rights reserved.
//

#ifndef OGRE_DEMO_HPP
#define OGRE_DEMO_HPP

#include "AdvancedOgreFramework.hpp"
#include "AppStateManager.hpp"

class Game
{
public:
	Game();
	~Game();
    
	void run();
    
private:
	AppStateManager*	mAppStateManager;
};


#endif
