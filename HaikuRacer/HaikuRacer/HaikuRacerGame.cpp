#include "HaikuRacerGame.h"
#include "AudioResourceManager.h"
#include <stdlib.h>
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
    over =false;
	new BtOgreFramework();
    BasicAudioSystem::getInstance();

    if (AudioResourceManager::getInstance().initialize()){

    AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("ambient");
    BasicAudioSystem::getInstance().playSound(u);
    
    AudioResource music = AudioResourceManager::getInstance().getResourceForEvent("music");
    BasicAudioSystem::getInstance().playSound(music);
    }
    
    scoreVal = 1;

	if(!BtOgreFramework::getSingletonPtr()->initOgre("One Game a Month January - HaikuRoller", this, 0))
		return;
    
    buttonGUI::textScheme myTextScheme("RosesAreFF0000",20, 1,1,1,1);
    btnManager = new buttonGUI::buttonManager("GUI/bgMaterial", myTextScheme, BtOgreFramework::getSingletonPtr()->m_pSceneMgr, "Camera");
    btnManager->createButton("title", "GUI/bgMaterial", buttonGUI::buttonPosition(buttonGUI::CENTER, 0,0), 1024,768);
    
    buttonGUI::button *scoreRoot = btnManager->createButton("gameplay", "GUI/none", buttonGUI::buttonPosition(buttonGUI::TOP_LEFT, 0,0), 0,0);
    score = scoreRoot->addTextArea("score", Ogre::UTFString("Score:"), 80, 50);
    score->hide(false);
    end = btnManager->createButton("end", "GUI/endScreen", buttonGUI::buttonPosition(buttonGUI::CENTER, 0,0), 1024,768);

    btnManager->getButton("end")->hide(false);
	m_bShutdown = false;
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
    BtOgreFramework::getSingletonPtr()->m_pTimer->reset();
	BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,275,75);
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
     if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_SPACE) ){
         if (!over){
             BtOgreFramework::getSingletonPtr()->m_UpdatePhysics = true;
             btnManager->getButton("title")->hide(false);
             score->show(false);
         }
         else{
             AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("fall_off");
             BasicAudioSystem::getInstance().stopSound(u);
             currentPanel = NULL;
             scoreVal = 1;
             over = false;
             end->hide(false);
             track->clear();
             score->show(false);
             setupGameScene();
             BtOgreFramework::getSingletonPtr()->m_pTimer->reset();
             BtOgreFramework::getSingletonPtr()->m_UpdatePhysics = true;

         }
     }
    return true;
}


bool HaikuRacerGame::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	BtOgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    if (!BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D) ){
        AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("apply_force_right");
        BasicAudioSystem::getInstance().stopSound(u);
    }
    if (!BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A) ){
        AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("apply_force_left");
        BasicAudioSystem::getInstance().stopSound(u);
    }
	return true;
}



struct CollisionOccurred : public btCollisionWorld::ContactResultCallback
{
    btCollisionObject *user;
    std::vector<TrackSegment> *t;
    TrackSegment **currentPanel;
    int *score;
    
    CollisionOccurred(btCollisionObject *vehicle, std::vector<TrackSegment> *track, TrackSegment **curPanelPointer, int *scorePtr){
        user = vehicle;
        t = track;
        currentPanel = curPanelPointer;
        score = scorePtr;
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
                    *score = *score+1;
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
                    *score = *score+1;
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
        
        Vector3 sub = ( vehicle->headNode->getPosition() - BtOgreFramework::getSingletonPtr()->m_pCamera->getPosition() );
        end->show(false);

        score->hide(false);
        vehicle->node->setPosition(0,2,0);
        vehicle->rigidBody->setGravity(btVector3(0,-99,0));
        end->show(false);
        over = true;
    }
    
    else{
        std::stringstream ss;
        ss << scoreVal;
        scoreString = UTFString("Score: "+ss.str());
        score->editTextArea("score", scoreString);
                
        btVector3 vehicleVelocity = vehicle->rigidBody->getLinearVelocity();
        btVector3 normVehicleVelocity = vehicleVelocity.normalize();
        
        vehicle->rigidBody->setLinearVelocity(normVehicleVelocity*(BtOgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds()/1000));
        
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
        forward = forward.normalize();
        Vector3 trackVel = Vector3(ogVelocity.x, 0, ogVelocity.z);
        
        float velocityLength = vehicleVelocity.length();
        
        if ( velocityLength > 0.1f){
          if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D)){
             
              AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("apply_force_right");
              BasicAudioSystem::getInstance().playSound(u);
              vehicle->rigidBody->applyCentralForce(right*3);
          }
        
            if (BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A)){
                AudioResource u = AudioResourceManager::getInstance().getResourceForEvent("apply_force_left");
                BasicAudioSystem::getInstance().playSound(u);
                vehicle->rigidBody->applyCentralForce(-right*3);
            }
        }
    
        
        if (velocityLength > 15+(BtOgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds()/1000)) vehicle->rigidBody->setLinearVelocity(normVehicleVelocity*15);
        
        CollisionOccurred handler(vehicle->rigidBody, &track->track, &currentPanel, &scoreVal);
        BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->contactTest(vehicle->rigidBody, handler);
    }
}

bool HaikuRacerGame::detectGameOver(){
    if ( currentPanel == NULL){
        return false;
    }
    bool end = vehicle->node->getPosition().y < track->getLowestY() - 15;
    
    return end;
}


