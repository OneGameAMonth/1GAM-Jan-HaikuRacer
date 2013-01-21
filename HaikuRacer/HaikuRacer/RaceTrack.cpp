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

    TrackSegment *seg;
    for(int i = 0; i < NUM_SEGMENTS; i++)
    {
        seg = new TrackSegment((i>0)?track[i-1]:NULL);
        track.push_back(seg);
    }
    
    endSegment = &track[NUM_SEGMENTS-1];
    endSegment->ent->setMaterialName("ground/end");

}

float RaceTrack::getLowestY(){
    TrackSegment *seg;
    seg = &track[NUM_SEGMENTS-1];
    return seg->floorNode->getPosition().y;
}

RaceTrack::~RaceTrack(){}