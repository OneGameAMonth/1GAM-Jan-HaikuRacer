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
    
    Entity *headEntity = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
    headEntity->setVisible(true);
    headNode = node->createChildSceneNode();
    headNode->scale(0.02, 0.02, 0.02);
    headNode->attachObject(headEntity);
    headEntity->setMaterialName("Ogre/Eyes");
    BtOgre::StaticMeshToShapeConverter converter(headEntity);
    btCollisionShape *mHeadShape = converter.createSphere();

    
    
    btScalar mass = 2;
    btVector3 inertia;
    mHeadShape->calculateLocalInertia(mass, inertia);

    BtOgre::RigidBodyState *headState = new BtOgre::RigidBodyState(node);
    
    
    
    //Create the Body.
    rigidBody = new btRigidBody(mass, headState, mHeadShape, inertia);
    rigidBody->setRestitution(10000);
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->addRigidBody(rigidBody);
    rigidBody->setGravity(btVector3(0,-15,0));

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
