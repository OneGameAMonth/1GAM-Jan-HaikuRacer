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
#define NUM_BUFFERS 3
#define NUM_SOURCES 1
#define BUFFER_SIZE 4096

class BasicAudioSystem
{
    
    ALCdevice *dev;
    ALCcontext *ctx;
    struct stat statbuf;
    
    ALuint buffer[NUM_BUFFERS];
    ALuint source[NUM_SOURCES];
    
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
    
    void playSound(AudioEvent unit);
    void playSound(std::string fileName, bool looping);
    void playSound(std::string fileName, bool looping, float gain);
    void playSound(std::string fileName, bool looping, float gain, float panning, float pitch);
private:
    BasicAudioSystem();
};

#endif 
