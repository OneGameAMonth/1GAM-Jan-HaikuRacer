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
    if (AudioResourceManager::getInstance().initialize()){

    AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("ambient");
    BasicAudioSystem::getInstance().playSound(u);
    
    AudioResource music = AudioResourceManager::getInstance().getResourceForEvent("music");
    BasicAudioSystem::getInstance().playSound(music);
    }

	if(!BtOgreFramework::getSingletonPtr()->initOgre("One Game a Month January - HaikuRoller", this, 0))
		return;
    
	m_bShutdown = false;
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
    BtOgreFramework::getSingletonPtr()->m_pTimer->reset();

	setupGameScene();
#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	runGame();
#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void HaikuRacerGame::setupGameScene()
{
    vehicle = new RaceVehicle();
    vehicle->translate(Vector3(0,1.5,0));
    vehicle->rigidBody->activate();
    track = new RaceTrack();
    currentPanel = &track->track[0];
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
	
	return true;
}


bool HaikuRacerGame::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	BtOgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

	return true;
}



struct CollisionOccurred : public btCollisionWorld::ContactResultCallback
{
    btCollisionObject *user;
    std::vector<TrackSegment> *t;
    TrackSegment **currentPanel;
    
    CollisionOccurred(btCollisionObject *vehicle, std::vector<TrackSegment> *track, TrackSegment **curPanelPointer){
        user = vehicle;
        t = track;
        currentPanel = curPanelPointer;
    }
    
	virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,int partId0,int index0,const btCollisionObjectWrapper* colObj1Wrap,int partId1,int index1)
	{
        
        const btCollisionObject *a = colObj0Wrap->getCollisionObject();
        const btCollisionObject *b = colObj1Wrap->getCollisionObject();
        TrackSegment *seg;
        
        if (a == user){
            for (int i = 0; i < t->size(); i++){
                seg = &t->at(i);
                if ( b == seg->rigidBody && seg->glowing==false){
                    seg->startGlow();
                    AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("next_panel");
                    BasicAudioSystem::getInstance().playSound(u);
                    *currentPanel = seg;
                }
            }
        }
        else if (b == user){
            for (int i = 0; i < t->size(); i++){
                 seg = &t->at(i);
                if ( a == seg->rigidBody && seg->glowing == false){
                    seg->startGlow();
                    AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("next_panel");
                    BasicAudioSystem::getInstance().playSound(u);
                    *currentPanel = seg;
                }
            }
        }
        
        return 0;
	}
};

void HaikuRacerGame::updateGame(){
    
    if ( detectGameOver() ){
        AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("fall_off");
        BasicAudioSystem::getInstance().playSound(u);
        
        vehicle->node->setPosition(0,2,0);
        vehicle->rigidBody->setGravity(btVector3(0,-99,0));
    }
    
    else{
        vehicle->rigidBody->activate();
        btVector3 vehicleVelocity = vehicle->rigidBody->getLinearVelocity();
        vehicle->rigidBody->setLinearVelocity(vehicle->rigidBody->getLinearVelocity().normalized()*+(BtOgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds()/1000));
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

        Vector3 trackVel = Vector3(ogVelocity.x, 0, ogVelocity.z);
        
        if ( vehicleVelocity.length() > 0.1f){
          if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D)){
              vehicle->rigidBody->activate();
              
              forward = forward.normalize();
              
              vehicle->rigidBody->applyCentralForce(right*forward.length()*3);
          }
        
        if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A)){
            vehicle->rigidBody->activate();
            forward = forward.normalize();

            vehicle->rigidBody->applyCentralForce(-right*forward.length()*3);
        }
        }
        
        
        if (vehicleVelocity.length() > 15+(BtOgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds()/1000)) vehicle->rigidBody->setLinearVelocity(vehicleVelocity.normalized()*15);
        
        CollisionOccurred handler(vehicle->rigidBody, &track->track, &currentPanel);
        BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->contactTest(vehicle->rigidBody, handler);
    }
}

bool HaikuRacerGame::detectGameOver(){
    if ( currentPanel == NULL){
        return false;
    }
    return vehicle->node->getPosition().y < track->getLowestY() - 20;
}


