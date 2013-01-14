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
    node = node->createChildSceneNode();
    
    Entity *headEntity = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("sphere.mesh");
    headEntity->setVisible(true);
    headNode = node->createChildSceneNode();
    headNode->attachObject(headEntity);
    headEntity->setMaterialName("Ogre/Eyes");
    BtOgre::StaticMeshToShapeConverter converter(headEntity);
    btCollisionShape *mHeadShape = converter.createSphere();


    btScalar mass = 20;
    btVector3 inertia;
    mHeadShape->calculateLocalInertia(mass, inertia);

    BtOgre::RigidBodyState *headState = new BtOgre::RigidBodyState(node);
    
    
    
    //Create the Body.
    rigidBody = new btRigidBody(mass, headState, mHeadShape, inertia);
    rigidBody->setGravity(btVector3(0,-10000,0));
    rigidBody->setRestitution(1000);
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->addRigidBody(rigidBody);
}


void RaceVehicle::translate(Vector3 t){
    node->translate(t);
    rigidBody->translate(btVector3(t.x,t.y,t.z));
}

void RaceVehicle::attachToSceneNode(Ogre::SceneNode *attachTo){
    attachTo->addChild(node);
}

void RaceVehicle::attachSceneNode(Ogre::SceneNode *attaching){
    node->addChild(attaching);
}

Vector3 RaceVehicle::getForwardVector(){
    
    return node->getOrientation().zAxis();
}
