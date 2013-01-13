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
    
public:
    SceneNode *headNode;
    Vector3 *forward;
public:
    SceneNode *node;
    btRigidBody *rigidBody;
    RaceVehicle();
    ~RaceVehicle(){}
    void attachToSceneNode(SceneNode *attachTo);
    void attachSceneNode(SceneNode *attaching);
    void translate(Ogre::Vector3 t);
Vector3 getForwardVector();

};



#endif /* defined(__OgreTest2__BasicTree__) */


