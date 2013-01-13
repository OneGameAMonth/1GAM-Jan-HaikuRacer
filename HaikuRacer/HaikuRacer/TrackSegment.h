//
//  TrackSegment.h
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-08.
//
//

#ifndef __HaikuRacer__TrackSegment__
#define __HaikuRacer__TrackSegment__

#include <iostream>
#include <Ogre/Ogre.h>
#include <OgreSceneManager.h>
#include "BtOgreFramework.h"
#include <string>

using namespace Ogre;

class TrackSegment{
public:
    float length;
    float rotation;
    
    SceneNode *node;
    SceneNode *floorNode;
    btRigidBody *rigidBody;
    float theta;
    float xTheta;
    TrackSegment *previousSegment;

    TrackSegment(TrackSegment *previous);
    ~TrackSegment(){}
    
};

#endif /* defined(__HaikuRacer__TrackSegment__) */
