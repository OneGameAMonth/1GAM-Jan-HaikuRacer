//
//  AudioResourceManager.h
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-06.
//
//

#ifndef __HaikuRacer__AudioResourceManager__
#define __HaikuRacer__AudioResourceManager__

#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include "rapidxml.hpp"
typedef struct{
    std::string filename;
    float pitch;
    float panning;
    float gain;
    bool looping;
    
}AudioEvent;

class AudioResourceManager
{
private:
    std::map<std::string, std::vector<AudioEvent> > eventUnits;
    AudioResourceManager(){}
public:
    static AudioResourceManager& getInstance()
    {
        static AudioResourceManager instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    void initialize();
    AudioEvent getUnitForEvent(std::string eventName);
};

#endif 
