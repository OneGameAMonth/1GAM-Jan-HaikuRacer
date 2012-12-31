//
//  Game.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 12-12-31.
//  Copyright (c) 2012 Team Chilly. All rights reserved.
//

#include "Game.h"

void Game::runGame(){
    new BtOgreFramework();
	if(!BtOgreFramework::getSingletonPtr()->initOgre("AdvancedOgreFramework", 0, 0))
		return;
    
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
    
	gameStateManager = new GameStateManager();
    
    
	gameStateManager->start(gameStateManager->findByName("MenuState"));
}

Game::Game(){
    gameStateManager = 0;
}

Game::~Game(){
    delete gameStateManager;
    delete BtOgreFramework::getSingletonPtr();
}