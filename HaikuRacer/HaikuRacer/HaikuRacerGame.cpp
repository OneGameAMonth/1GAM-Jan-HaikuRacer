#include "HaikuRacerGame.h"

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
    
	if(!BtOgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, 0))
		return;
    
	m_bShutdown = false;
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
	
	setupGameScene();
#if !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	runDemo();
#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void HaikuRacerGame::setupGameScene()
{
    vehicle = new RaceVehicle();
    vehicle->node->translate(0, 0.8, 0);
    
	BtOgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
    
	BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,75,75);

    Entity *ground = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("testGround", Ogre::SceneManager::PT_CUBE);
    ground->setMaterialName("Environment/TrackTile");
    SceneNode *groundNode = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
    groundNode->attachObject(ground);
    groundNode->setScale(0.1, 0.001, 0.2);
    groundNode->setPosition(0, -0.1, 5);
    
    BtOgre::StaticMeshToShapeConverter converter(ground);
    btCollisionShape *groundShape = converter.createBox();
    
    Entity *leftWall = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(Ogre::SceneManager::PT_CUBE);
    SceneNode *lWallNode = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
    leftWall->setMaterialName("Environment/TrackTile");
    lWallNode->setScale(0.01, 0.02, 0.2);
    lWallNode->attachObject(leftWall);
    lWallNode->setPosition(5.5, 1, 5);
    
    Entity *rightWall = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity(Ogre::SceneManager::PT_CUBE);
    SceneNode *rWallNode = BtOgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
    rightWall->setMaterialName("Environment/TrackTile");

    rWallNode->setScale(0.01, 0.02, 0.2);
    rWallNode->attachObject(rightWall);
    rWallNode->setPosition(-5.5, 1, 5);
    
    btScalar mass = 0;
    btVector3 inertia;
    groundShape->calculateLocalInertia(mass, inertia);
    
    BtOgre::RigidBodyState *headState = new BtOgre::RigidBodyState(groundNode);
    
    //Create the Body.
    btRigidBody *groundRigid = new btRigidBody(mass, headState, groundShape, btVector3(0,0,0));
    BtOgreFramework::getSingletonPtr()->m_pPhysicsWorld->addRigidBody(groundRigid);
    groundRigid->setGravity(btVector3(0,0,0));

    
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void HaikuRacerGame::runGame()
{
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
	
	double timeSinceLastFrame = 0;
	double startTime = 0;
    
    BtOgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
    /*
#if (!defined(OGRE_IS_IOS)) && !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__)
	while(!m_bShutdown && !BtOgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{
		if(BtOgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;
        
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		Ogre::WindowEventUtilities::messagePump();
#endif	
		if(BtOgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			startTime = BtOgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
            
			BtOgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			BtOgreFramework::getSingletonPtr()->m_pMouse->capture();
            
			BtOgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			BtOgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
            
			timeSinceLastFrame = BtOgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
    
	}
#endif*/
    
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	BtOgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}


bool HaikuRacerGame::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	BtOgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
	{        
        vehicle->rigidBody->activate();
        vehicle->rigidBody->setLinearVelocity(btVector3(0, 0, 10));
    }
	return true;
}


bool HaikuRacerGame::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	BtOgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    
    if(!BtOgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
	{
        vehicle->rigidBody->setLinearVelocity(btVector3(0, 0, 0));
    }

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||
