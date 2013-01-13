//
//  TrackSegment.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-08.
//
//

#include "TrackSegment.h"


TrackSegment::TrackSegment(TrackSegment *previous){
    
    node = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
   

    Entity *floorEntity = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(Ogre::SceneManager::PT_CUBE);
    floorEntity->setVisible(true);
    floorNode = node->createChildSceneNode();
    
    floorNode->attachObject(floorEntity);
    floorNode->setScale(0.1, 0.01, 0.2);
    floorEntity->setMaterialName("Ogre/Skin");
    
    
    if ( previous != NULL){
        previousSegment = previous;
        
        
        theta = previousSegment->theta + rand()%60-30;
        
        xTheta = previousSegment->xTheta + rand()%20-10;
        if ( xTheta < 0 )xTheta*=-1;
        if ( xTheta > 60)xTheta-=(xTheta-60);
        
        floorNode->setPosition(previousSegment->floorNode->getPosition());
        floorNode->setOrientation(previousSegment->floorNode->getOrientation());
        
        if (theta > 0 )floorNode->translate(Vector3(-3.5,0,9.5), SceneNode::TS_LOCAL);
        else floorNode->translate(Vector3(3.5,0,9.5), SceneNode::TS_LOCAL);
        
        

        //rotate / position the segment
        floorNode->setOrientation(Quaternion::IDENTITY);
        floorNode->rotate(Vector3(0,1,0),Radian(Degree(theta)),Ogre::SceneNode::TS_LOCAL);
        floorNode->rotate(Vector3(1,0,0),Radian(Degree(xTheta)), SceneNode::TS_LOCAL);
        
        
       if ( theta > 0)floorNode->translate(Vector3(3.5,0,9.7), SceneNode::TS_LOCAL);
       else floorNode->translate(Vector3(-3.5,0,9.7), SceneNode::TS_LOCAL);

       
    }
    else{
        theta = 18;
        xTheta = 18;
        floorNode->rotate(Vector3(1,0,0),Radian(Degree(18)), SceneNode::TS_LOCAL);

    }
    
    
    
    BtOgre::StaticMeshToShapeConverter converter(floorEntity);
    btCollisionShape *mFloorShape = converter.createBox();
    
    
    btScalar mass = 0;
    btVector3 inertia;
    mFloorShape->calculateLocalInertia(mass, inertia);
    
    

    
    BtOgre::RigidBodyState *floorState = new BtOgre::RigidBodyState(floorNode);
    
    //Create the Body.
    rigidBody = new btRigidBody(mass, floorState, mFloorShape, inertia);
    
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->addRigidBody(rigidBody);
 
}
