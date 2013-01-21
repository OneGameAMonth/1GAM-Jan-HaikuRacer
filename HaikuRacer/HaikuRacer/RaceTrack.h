//
//  RaceTrack.h
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-08.
//
//

#ifndef __HaikuRacer__RaceTrack__
#define __HaikuRacer__RaceTrack__

#include <iostream>
#include <Ogre/Ogre.h>
#include <OgreSceneManager.h>
#include "BtOgreFramework.h"
#include <string>
#include "TrackSegment.h"

#define NUM_SEGMENTS 400
using namespace Ogre;

class RaceTrack{
public:
    SceneNode *node;
    std::vector<TrackSegment> track;
    
    TrackSegment *endSegment;
    
    float getLowestY();
    RaceTrack();
    ~RaceTrack();

};

#endif /* defined(__HaikuRacer__RaceTrack__) */
