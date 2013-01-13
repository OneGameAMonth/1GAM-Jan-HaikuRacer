//
//  RaceTrack.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-08.
//
//

#include "RaceTrack.h"

RaceTrack::RaceTrack(){
    node = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();


    for(int i = 0; i < NUM_SEGMENTS; i++)
    {
        TrackSegment *seg = new TrackSegment((i>0)?track[i-1]:NULL);
        track.push_back(seg);
     //   node->addChild(seg->floorNode);
    }
}

RaceTrack::~RaceTrack(){}