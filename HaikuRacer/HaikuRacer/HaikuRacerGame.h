#ifndef OGRE_DEMO_H
#define OGRE_DEMO_H

#include "BtOgreFramework.h"
#include "RaceVehicle.h"
#include "RaceTrack.h"
class HaikuRacerGame : public OIS::KeyListener
{
public:
	HaikuRacerGame();
	~HaikuRacerGame();
    
	void startGame();
    void updateGame();
	
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
    
private:
    void setupGameScene();
	void runGame();

	Ogre::SceneNode*			m_pCubeNode;
	Ogre::Entity*				m_pCubeEntity;
    
	bool					m_bShutdown;
    
    RaceVehicle *vehicle;
    RaceTrack *track;
};


#endif 

