//
//  main.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 12-12-31.
//  Copyright (c) 2012 Team Chilly. All rights reserved.
//


#include "Game.h"

int main(int argc, char **argv)
{
	Game app;
	try
	{
		app.runGame();
	}
	catch(std::exception& e)
    {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }
    
    return 0;
}
