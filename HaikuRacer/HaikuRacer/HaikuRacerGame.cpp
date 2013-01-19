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

    BtOgreFramework::getSingletonPtr()->m_pCamera->setAutoTracking(true,vehicle->node);
    
	BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,275,75);
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
    std::vector<TrackSegment> *t;
    TrackSegment *currentPanel;
    
    CollisionOccurred(btCollisionObject *vehicle, std::vector<TrackSegment> *track, TrackSegment *curPanelPointer){
        user = vehicle;
        t = track;
        curPanelPointer = currentPanel;
    }
    
	virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1)
	{
        
        const btCollisionObject *a = colObj0Wrap->getCollisionObject();
        const btCollisionObject *b = colObj1Wrap->getCollisionObject();
        
        if (a == user){
            for (int i = 0; i < t->size(); i++){
                TrackSegment *seg = &t->at(i);
                if ( b == seg->rigidBody && seg->glowing==false){
                    seg->startGlow();
                    AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("next_panel");
                    BasicAudioSystem::getInstance().playSound(u);
                    currentPanel = seg;

                    
                }
            }
        }
        else if (b == user){
            for (int i = 0; i < t->size(); i++){
                TrackSegment *seg = &t->at(i);
                if ( a == seg->rigidBody && seg->glowing == false){
                    seg->startGlow();
                    AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("next_panel");
                    BasicAudioSystem::getInstance().playSound(u);
                    currentPanel = seg;
                }
            }
        }
        
        
        
        return 0;
	}
};

void HaikuRacerGame::updateGame(){
    vehicle->rigidBody->activate();
    btVector3 vehicleVelocity = vehicle->rigidBody->getLinearVelocity();
    
    Vector3 ogVelocity = Vector3(vehicleVelocity.x(), vehicleVelocity.y(), vehicleVelocity.z());
    Vector2 og2D = Vector2(ogVelocity.x, ogVelocity.z);
    og2D = og2D.normalisedCopy();
    
    Vector3 pos = Vector3(vehicle->node->getPosition()-Vector3(og2D.x*9, -25, og2D.y*9));
    
    lookVectors.push_back(pos);
    
    while (lookVectors.size() > 25){
        lookVectors.pop_front();
    }
    pos = Vector3(0,0,0);
    for (int i = 0; i < lookVectors.size(); i++){
        pos += lookVectors[i];
    }
    
    pos = pos/lookVectors.size();
    
   


    BtOgreFramework::getSingletonPtr()->m_pCamera->setPosition(pos);
    btVector3 forward = btVector3(vehicle->rigidBody->getLinearVelocity().x(), 0, vehicle->rigidBody->getLinearVelocity().z());
    btVector3 right = (forward.rotate(btVector3(0,1,0), -90));

   // BtOgreFramework::getSingletonPtr()->m_pCamera->lookAt(vehicle->node->getPosition()+Vector3(forward.x(), forward.y()-2, forward.z()));

    Vector3 trackVel = Vector3(ogVelocity.x, 0, ogVelocity.z);
    
      if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D)){
          vehicle->rigidBody->activate();
          
          forward = forward.normalize();
          
          //vehicle->rigidBody->setLinearVelocity(vehicle->rigidBody->getLinearVelocity().rotate(up, forward.length()*0.01f));
          vehicle->rigidBody->applyCentralForce(right*forward.length()*3);
      }
    
    if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A)){
        vehicle->rigidBody->activate();
        forward = forward.normalize();

        vehicle->rigidBody->applyCentralForce(-right*forward.length()*3);
        //vehicle->rigidBody->applyCentralForce(right*150);
    }
    
    
    if (vehicleVelocity.length() > 15) vehicle->rigidBody->setLinearVelocity(vehicleVelocity.normalized()*15);
    CollisionOccurred handler(vehicle->rigidBody, &track->track, currentPanel);
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->contactTest(vehicle->rigidBody, handler);
    
}


