#include "BtOgreFramework.h"
#include "macUtils.h"

using namespace Ogre; 

namespace Ogre
{
    template<> BtOgreFramework* Ogre::Singleton<BtOgreFramework>::msSingleton = 0;
};

BtOgreFramework::BtOgreFramework()
{
	m_MoveSpeed			= 0.1f;
	m_RotateSpeed       = 0.3f;
    
	m_bShutDownOgre     = false;
	m_iNumScreenShots   = 0;
    
	m_pRoot				= 0;
	m_pSceneMgr			= 0;
	m_pRenderWnd        = 0;
	m_pCamera			= 0;
	m_pViewport			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;
    
	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
    m_pTrayMgr          = 0;
    m_FrameEvent        = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||
#if defined(OGRE_IS_IOS)
bool BtOgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MultiTouchListener *pMouseListener)
#else
bool BtOgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
#endif
{
    new Ogre::LogManager();

	m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);
    
    String pluginsPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
    pluginsPath = m_ResourcePath + "plugins.cfg";
#endif
    
    m_pRoot = new Ogre::Root(pluginsPath, Ogre::macBundlePath() + "/ogre.cfg");
    
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.load();
#endif
    
	//if(!m_pRoot->showConfigDialog())
	//	return false;
    
    RenderSystem *rs = m_pRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
    // or use "OpenGL Rendering Subsystem"
    m_pRoot->setRenderSystem(rs);
    rs->setConfigOption("Full Screen", "No");
    rs->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
    rs->setConfigOption("FSAA", "2");

	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
    
	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	
	m_pCamera = m_pSceneMgr->createCamera("Camera");
	m_pCamera->setPosition(Vector3(0, 8, -15));
	m_pCamera->lookAt(Vector3(0, 0, 20));
	m_pCamera->setNearClipDistance(1);
    
	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
	m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
	m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));
	
	m_pViewport->setCamera(m_pCamera);
    
	unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);
    
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
    
	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
    
    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
    
	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();
    
	if(pKeyListener == 0)
		m_pKeyboard->setEventCallback(this);
	else
		m_pKeyboard->setEventCallback(pKeyListener);
    
	if(pMouseListener == 0)
		m_pMouse->setEventCallback(this);
	else
		m_pMouse->setEventCallback(pMouseListener);
    
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load(m_ResourcePath + "resources.cfg");
    
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || defined(OGRE_IS_IOS)
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
                archName = Ogre::String(m_ResourcePath + archName);
#endif
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
	m_pTimer = OGRE_NEW Ogre::Timer();
	m_pTimer->reset();
	
	m_pTrayMgr = new OgreBites::SdkTrayManager("TrayMgr", m_pRenderWnd, m_pMouse, this);
    m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    m_pTrayMgr->hideCursor();
    
	m_pRenderWnd->setActive(true);
    
    
    mBroadphase = new btAxisSweep3(btVector3(-10000,-10000,-10000), btVector3(10000,10000,10000), 1024);
    mCollisionConfig = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfig);
    mSolver = new btSequentialImpulseConstraintSolver();
    
    m_pPhysicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfig);
    m_pPhysicsWorld->setGravity(btVector3(0,-9.8,0));
    
    dbgdraw = new BtOgre::DebugDrawer(m_pSceneMgr->getRootSceneNode(), m_pPhysicsWorld);
    dbgdraw->setDebugMode(1);
    
	return true;
}

BtOgreFramework::~BtOgreFramework()
{
    if(m_pInputMgr) OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pTrayMgr)  delete m_pTrayMgr;
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.unload();
#endif
    if(m_pRoot)     delete m_pRoot;
}

bool BtOgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	
	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
        m_bShutDownOgre = true;
        return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		m_pRenderWnd->writeContentsToTimestampedFile("BOF_Screenshot_", ".png");
		return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		
		if(mode == 2)
		{
			m_pCamera->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if(mode == 0)
		{
            m_pCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
		}
		else if(mode == 1)
		{
			m_pCamera->setPolygonMode(PM_POINTS);
			mode = 2;
		}
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		if(m_pTrayMgr->isLogoVisible())
        {
            m_pTrayMgr->hideLogo();
            m_pTrayMgr->hideFrameStats();
        }
        else
        {
            m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        }
	}
    return true;
}

bool BtOgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

bool BtOgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
	m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
	
	return true;
}

bool BtOgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

bool BtOgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

void BtOgreFramework::updateOgre(double timeSinceLastFrame)
{
	m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;

#if OGRE_VERSION >= 0x10800
    m_pSceneMgr->setSkyBoxEnabled(true);
#endif
    m_pPhysicsWorld->stepSimulation(timeSinceLastFrame);
    m_pPhysicsWorld->debugDrawWorld();
    dbgdraw->step();
	m_TranslateVector = Vector3::ZERO;
    
	getInput();
	moveCamera();
    
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
}

void BtOgreFramework::moveCamera()
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)) 
		m_pCamera->moveRelative(m_TranslateVector);
	else
#endif
		m_pCamera->moveRelative(m_TranslateVector / 10);
}

void BtOgreFramework::getInput()
{
#if !defined(OGRE_IS_IOS)
/*	if(m_pKeyboard->isKeyDown(OIS::KC_A))
		m_TranslateVector.x = -m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_D))
		m_TranslateVector.x = m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_W))
		m_TranslateVector.z = -m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_S))
		m_TranslateVector.z = m_MoveScale;*/
#endif
}
