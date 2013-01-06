//
//  AudioResourceManager.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-06.
//
//

#include "AudioResourceManager.h"
#include "macUtils.h"

using namespace rapidxml;

void AudioResourceManager::initialize()
{
    printf("initializing audio resource manager");
    Ogre::String path = Ogre::macBundlePath() + "/Contents/Resources/media/config/audio_events.xml";
    xml_document<> doc;    // character type defaults to char
    printf("Opening config file at path %s", path.data());
    FILE *audioConfigFile = fopen(path.data(), "r");
    
    
    std::vector<char> buffer;
    int c;
    while (EOF !=(c=getc(audioConfigFile))){
        buffer.push_back(c);
    }
    
    char buf[buffer.size()];
    for (int i = 0; i < buffer.size(); i++){
        buf[i] = buffer[i];
    }
    
    doc.parse<0>(buf); // 0 means default parse flags
    
    
    
    //get the first event
    xml_node<>* eventIter = doc.first_node()->first_node();

    while (eventIter != NULL)
    {
        xml_node<>* unitIter = eventIter->first_node()->next_sibling();
        std::string eventName = eventIter->first_node()->value();
        while (unitIter != NULL) {
            AudioEvent unit;
            
            unit.filename = unitIter->first_node("filename")->value();
            unit.panning = atof(unitIter->first_node("panning")->value());
            unit.pitch = atof(unitIter->first_node("pitch")->value());
            unit.looping = atoi(unitIter->first_node("looping")->value());
            unit.gain = atof(unitIter->first_node("gain")->value());
            
            eventUnits[eventName].push_back(unit);
            
            unitIter = unitIter->next_sibling();
        }
        
        eventIter = eventIter->next_sibling();
        
    }
}

AudioEvent AudioResourceManager::getUnitForEvent(std::string eventName){
    return eventUnits[eventName][0];
}
