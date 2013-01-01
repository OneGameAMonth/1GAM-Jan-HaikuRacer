//
//  BasicTree.h
//  OgreTest2
//
//  Created by Kyle Halladay on 12-11-21.
//
//

#ifndef RACEVEHICLE_H
#define RACEVEHICLE_H
#include <iostream>
#include <Ogre/Ogre.h>
#include <OgreSceneManager.h>
#include "BtOgreFramework.h"
#include <string>

using namespace Ogre;

class RaceVehicle{
    
private:
    SceneNode *headNode;
    
public:
    SceneNode *node;
    
    RaceVehicle();
    ~RaceVehicle(){}
    void attachToSceneNode(SceneNode *attachTo);
    void attachSceneNode(SceneNode *attaching);
    
};

#endif /* defined(__OgreTest2__BasicTree__) */


