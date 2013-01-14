#include "HaikuRacerGame.h"
#include "AudioResourceManager.h"
HaikuRacerGame::HaikuRacerGame()
{
	m_pCubeNode		= 0;
	m_pCubeEntity   = 0;
}

HaikuRacerGame::~HaikuRacerGame()
{
    delete BtOgreFramework::getSingletonPtr();
}

void HaikuRacerGame::startGame()
{
	new BtOgreFramework();
    AudioResourceManager::getInstance().initialize();

    AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("ambient");
    BasicAudioSystem::getInstance().playSound(u);

	if(!BtOgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, 0))
		return;
    
	m_bShutdown = false;
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
	
	setupGameScene();
#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	runGame();
#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void HaikuRacerGame::setupGameScene()
{
    vehicle = new RaceVehicle();
    vehicle->translate(Vector3(0,10,0));
    vehicle->rigidBody->activate();
    track = new RaceTrack();

    
	//BtOgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    
	BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,75,75);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void HaikuRacerGame::runGame()
{
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
	
	double timeSinceLastFrame = 0;
	double startTime = 0;
    
    BtOgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();

	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}


bool HaikuRacerGame::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	BtOgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
	{

        vehicle->rigidBody->activate();
        vehicle->rigidBody->setLinearVelocity(btVector3(0, 0, -10));
    }
    
	return true;
}


bool HaikuRacerGame::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	BtOgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    
    if(!BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
	{
      //  vehicle->rigidBody->setLinearVelocity(btVector3(0, 0, 0));
    }

	return true;
}


struct CollisionOccurred : public btCollisionWorld::ContactResultCallback
{
    btCollisionObject *user;
    std::vector<TrackSegment> t;
    
    CollisionOccurred(btCollisionObject *vehicle, std::vector<TrackSegment> track){
        user = vehicle;
        t = track;
    }
    
	virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1)
	{
        
        const btCollisionObject *a = colObj0Wrap->getCollisionObject();
        const btCollisionObject *b = colObj1Wrap->getCollisionObject();
        
        if (a == user){
            for (int i = 0; i < t.size(); i++){
                TrackSegment seg = t[i];
                if ( b == seg.rigidBody) seg.startGlow();
            }
        }
        else if (b == user){
            for (int i = 0; i < t.size(); i++){
                TrackSegment seg = t[i];
                if ( a == seg.rigidBody) seg.startGlow();
            }
        }
        
        return 0;
	}
};

void HaikuRacerGame::updateGame(){
    vehicle->rigidBody->activate();
    btVector3 vehicleVelocity = vehicle->rigidBody->getLinearVelocity();
    Vector3 ogVelocity = Vector3(vehicleVelocity.x(), vehicleVelocity.y(), vehicleVelocity.z());
    ogVelocity = ogVelocity.normalisedCopy();
    BtOgreFramework::getSingletonPtr()->m_pCamera->setPosition(vehicle->node->getPosition()+Vector3(0,30,0));
   // BtOgreFramework::getSingletonPtr()->m_pCamera->lookAt(vehicle->node->getPosition());
    Vector3 trackVel = Vector3(ogVelocity.x, 0, ogVelocity.z);
    
   // BtOgreFramework::getSingletonPtr()->m_pCamera->lookAt(vehicle->node->getPosition());
    
    if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D)){
        vehicle->rigidBody->activate();
        vehicle->rigidBody->applyCentralForce(btVector3(-150,0,0));
    }
    
    if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A)){
        vehicle->rigidBody->activate();
       // vehicle->rigidBody->applyCentralImpulse(btVector3(10, 0, 0));
        vehicle->rigidBody->applyCentralForce(btVector3(150,0,0));
    }
    if (vehicleVelocity.length() > 10) vehicle->rigidBody->setLinearVelocity(vehicleVelocity.normalized()*10);
    CollisionOccurred handler(vehicle->rigidBody, track->track);
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->contactTest(vehicle->rigidBody, handler);
    
}


