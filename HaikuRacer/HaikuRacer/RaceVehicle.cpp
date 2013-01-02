//
//  RaceVehicle.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-01.
//
//

#include "RaceVehicle.h"


RaceVehicle::RaceVehicle(){
    
    node = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode();

    Entity *headEntity = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("sphere.mesh");
    headEntity->setVisible(true);
    headNode = node->createChildSceneNode();
    headNode->attachObject(headEntity);
    
    BtOgre::StaticMeshToShapeConverter converter(headEntity);
    btCollisionShape *mHeadShape = converter.createSphere();


    btScalar mass = 5;
    btVector3 inertia;
    mHeadShape->calculateLocalInertia(mass, inertia);

    BtOgre::RigidBodyState *headState = new BtOgre::RigidBodyState(headNode);
    
    //Create the Body.
    rigidBody = new btRigidBody(mass, headState, mHeadShape, inertia);
    
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->addRigidBody(rigidBody);
}


void RaceVehicle::attachToSceneNode(Ogre::SceneNode *attachTo){
    attachTo->addChild(node);
}

void RaceVehicle::attachSceneNode(Ogre::SceneNode *attaching){
    node->addChild(attaching);
}