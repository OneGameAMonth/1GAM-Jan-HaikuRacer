#ifndef OGRE_DEMO_H
#define OGRE_DEMO_H

#include "BtOgreFramework.h"
#include "RaceVehicle.h"
#include "RaceTrack.h"
#include "buttonGUI.h"
#include <vector>
#include <deque>
#include <stdlib.h>
#define Vector3 Ogre::Vector3

using namespace Ogre;
class HaikuRacerGame : public OIS::KeyListener
{
public:
	HaikuRacerGame();
	~HaikuRacerGame();
    
	void startGame();
    void updateGame();
	
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
    bool detectGameOver();
    
private:
    void setupGameScene();
	void runGame();
    bool leftDown, rightDown;
	Ogre::SceneNode*			m_pCubeNode;
	Ogre::Entity*				m_pCubeEntity;
    
    bool over;
    int scoreVal;
    UTFString scoreString;
	bool					m_bShutdown;
    std::deque<Vector3> lookVectors;
    RaceVehicle *vehicle;
    TrackSegment *currentPanel;
    RaceTrack *track;
    buttonGUI::button *score;
    buttonGUI::button *end;
    buttonGUI::buttonManager *btnManager;
};


#endif 

