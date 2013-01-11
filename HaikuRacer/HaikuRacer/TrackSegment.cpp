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
    floorNode->setScale(0.1, 0.001, 0.2);
  
    BtOgre::StaticMeshToShapeConverter converter(floorEntity);
    btCollisionShape *mFloorShape = converter.createBox();
    
    
    btScalar mass = 0;
    btVector3 inertia;
    mFloorShape->calculateLocalInertia(mass, inertia);
    
    BtOgre::RigidBodyState *floorState = new BtOgre::RigidBodyState(floorNode);
    
    //Create the Body.
    rigidBody = new btRigidBody(mass, floorState, mFloorShape, inertia);
    
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->addRigidBody(rigidBody);
    
    if ( previous != NULL){
        previousSegment = previous;
        node->translate(previousSegment->node->getPosition()+Vector3(0,0,20));
    
        //rotate / position the segment
        float theta = rand()%40;
        node->rotate(Vector3(0,1,0),Radian(Degree(theta)));
    }
    
    
 
}
