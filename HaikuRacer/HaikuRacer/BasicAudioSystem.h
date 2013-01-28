//
//  BasicAudioSystem.h
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-02.
//
//

#ifndef __HaikuRacer__BasicAudioSystem__
#define __HaikuRacer__BasicAudioSystem__

#include <string>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "AudioResourceManager.h"
#define NUM_BUFFERS 10
#define NUM_SOURCES 3
#define BUFFER_SIZE 2048

class BasicAudioSystem
{
    
    ALCdevice *dev;
    ALCcontext *ctx;
    struct stat statbuf;
        
    ALsizei size, freq;
    ALenum format;
    ALvoid *data;
    
public:
    static BasicAudioSystem& getInstance()
    {
        static BasicAudioSystem instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    
    void playSound(AudioResource unit);
    void stopSound(AudioResource unit);
    
private:
    BasicAudioSystem();
};

#endif 
