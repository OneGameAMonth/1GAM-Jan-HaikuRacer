//
//  AudioResourceManager.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-06.
//
//

#include "AudioResourceManager.h"
#include "macUtils.h"


using namespace tinyxml2;
bool AudioResourceManager::initialize()
{
    srand(time(NULL));
    Ogre::String path = Ogre::macBundlePath() + "/Contents/Resources/media/config/audio_events.xml";
    //xml_document<> doc;    // character type defaults to char
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
    XMLDocument doc;
    doc.Parse(buf);

    XMLElement *eventIter = doc.FirstChildElement()->FirstChildElement();
    
    
    unsigned int currentHandle = 0;
    
    while (eventIter != NULL)
    {
        XMLNode *unitIter = eventIter->FirstChildElement()->NextSiblingElement();
        std::string eventName = eventIter->FirstChildElement()->GetText();
        
        while (unitIter != NULL) {
            AudioResource unit;
            
            unit.filename = unitIter->FirstChildElement("filename")->GetText();// first_node("filename")->value();
            printf("filename: %s\n", unit.filename.c_str());
            unit.panning = atof(unitIter->FirstChildElement("panning")->GetText());
            unit.pitch = atof(unitIter->FirstChildElement("pitch")->GetText());
            unit.looping = atoi(unitIter->FirstChildElement("looping")->GetText());
            unit.gain = atof(unitIter->FirstChildElement("gain")->GetText());
            unit.bufferHandle = -1;
            unit.sourceHandle = -1;
            if ( eventName == "apply_force_left" || eventName == "apply_force_right" || eventName=="fall_off") unit.canStop = false;
            else unit.canStop = true;
            alGenSources(1, &unit.sourceHandle);
            resources[eventName].push_back(unit);
            
            unitIter = unitIter->NextSibling();
        }
        
        eventIter = eventIter->NextSiblingElement();
    }
    return true;
}

AudioResource AudioResourceManager::getResourceForEvent(std::string eventName){
    return resources[eventName][rand()%resources[eventName].size()];
}
