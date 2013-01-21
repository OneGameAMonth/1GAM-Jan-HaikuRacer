//
//  BasicAudioSystem.cpp
//  HaikuRacer
//
//  Created by Kyle Halladay on 13-01-02.
//
//

#include "BasicAudioSystem.h"
#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include "macUtils.h"
static bool LoadWAVFile(const char* filename, ALenum* format, ALvoid** data, ALsizei* size, ALsizei* freq, Float64* estimatedDurationOut)
{
    CFStringRef filenameStr = CFStringCreateWithCString( NULL, filename, kCFStringEncodingUTF8 );
    CFURLRef url = CFURLCreateWithFileSystemPath( NULL, filenameStr, kCFURLPOSIXPathStyle, false );
    CFRelease( filenameStr );
    
    AudioFileID audioFile;
    OSStatus error = AudioFileOpenURL( url, kAudioFileReadPermission, kAudioFileWAVEType, &audioFile );
    CFRelease( url );
    
    if ( error != noErr )
    {
        fprintf( stderr, "Error opening audio file. %d\n", error );
        return false;
    }
    
    AudioStreamBasicDescription basicDescription;
    UInt32 propertySize = sizeof(basicDescription);
    error = AudioFileGetProperty( audioFile, kAudioFilePropertyDataFormat, &propertySize, &basicDescription );
    
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading audio file basic description. %d\n", error );
        AudioFileClose( audioFile );
        return false;
    }
    
    if ( basicDescription.mFormatID != kAudioFormatLinearPCM )
    {
        // Need PCM for Open AL. WAVs are (I believe) by definition PCM, so this check isn't necessary. It's just here
        // in case I ever use this with another audio format.
        fprintf( stderr, "Audio file is not linear-PCM. %d\n", basicDescription.mFormatID );
        AudioFileClose( audioFile );
        return false;
    }
    
    UInt64 audioDataByteCount = 0;
    propertySize = sizeof(audioDataByteCount);
    error = AudioFileGetProperty( audioFile, kAudioFilePropertyAudioDataByteCount, &propertySize, &audioDataByteCount );
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading audio file byte count. %d\n", error );
        AudioFileClose( audioFile );
        return false;
    }
    
    Float64 estimatedDuration = 0;
    propertySize = sizeof(estimatedDuration);
    error = AudioFileGetProperty( audioFile, kAudioFilePropertyEstimatedDuration, &propertySize, &estimatedDuration );
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading estimated duration of audio file. %d\n", error );
        AudioFileClose( audioFile );
        return false;
    }
    
    ALenum alFormat = 0;
    
    if ( basicDescription.mChannelsPerFrame == 1 )
    {
        if ( basicDescription.mBitsPerChannel == 8 )
            alFormat = AL_FORMAT_MONO8;
        else if ( basicDescription.mBitsPerChannel == 16 )
            alFormat = AL_FORMAT_MONO16;
        else
        {
            fprintf( stderr, "Expected 8 or 16 bits for the mono channel but got %d\n", basicDescription.mBitsPerChannel );
            AudioFileClose( audioFile );
            return false;
        }
        
    }
    else if ( basicDescription.mChannelsPerFrame == 2 )
    {
        if ( basicDescription.mBitsPerChannel == 8 )
            alFormat = AL_FORMAT_STEREO8;
        else if ( basicDescription.mBitsPerChannel == 16 )
            alFormat = AL_FORMAT_STEREO16;
        else
        {
            fprintf( stderr, "Expected 8 or 16 bits per channel but got %d\n", basicDescription.mBitsPerChannel );
            AudioFileClose( audioFile );
            return false;
        }
    }
    else
    {
        fprintf( stderr, "Expected 1 or 2 channels in audio file but got %d\n", basicDescription.mChannelsPerFrame );
        AudioFileClose( audioFile );
        return false;
    }
    
    UInt32 numBytesToRead = audioDataByteCount;
    void* buffer = malloc( numBytesToRead );
    
    if ( buffer == NULL )
    {
        fprintf( stderr, "Error allocating buffer for audio data of size %u\n", numBytesToRead );
        return false;
    }
    
    error = AudioFileReadBytes( audioFile, false, 0, &numBytesToRead, buffer );
    AudioFileClose( audioFile );
    
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading audio bytes. %d\n", error );
        free(buffer);
        return false;
    }
    
    if ( numBytesToRead != audioDataByteCount )
    {
        fprintf( stderr, "Tried to read %lld bytes from the audio file but only got %d bytes\n", audioDataByteCount, numBytesToRead );
        free(buffer);
        return false;
    }
    
    *freq = basicDescription.mSampleRate;
    *size = audioDataByteCount;
    *format = alFormat;
    *data = buffer;
    *estimatedDurationOut = estimatedDuration;
    
    return true;
}



BasicAudioSystem::BasicAudioSystem()
{
    // Initialization
    dev = alcOpenDevice(NULL); // select the "preferred dev"
    
    if (dev)
    {
        ctx = alcCreateContext(dev,NULL);
        alcMakeContextCurrent(ctx);
    }
   
    alGetError(); // clear error code
        
}

static int curBuffer = 0;

void BasicAudioSystem::playSound(AudioResource unit){
    char*     alBuffer;             //data for the buffer
    ALenum alFormatBuffer;    //buffer format
    ALsizei   alFreqBuffer;       //frequency
    long       alBufferLen;        //bit depth
    ALboolean    alLoop;         //loop
    unsigned int alSource;      //source
    unsigned int alSampleSet;
    
    Float64 duration;
    
    
    // Load test.wav
    Ogre::String path = Ogre::macBundlePath() + "/Contents/Resources/media/audio/"+unit.filename;
    LoadWAVFile(path.data(), &format, &data, &size, &freq, &duration);
    ALenum error;
    
    
    if ( unit.bufferHandle == NULL) alGenBuffers(1, &unit.bufferHandle);

    if ((error = alGetError()) != AL_NO_ERROR)
    {
        alDeleteBuffers(NUM_BUFFERS, &unit.bufferHandle);
        return 1;
    }
    
    // Copy test.wav data into AL Buffer 0
    alBufferData(unit.bufferHandle, format, data, size, freq);
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        alDeleteBuffers(NUM_BUFFERS, &unit.bufferHandle);
        return 1;
    }
    if ( unit.sourceHandle == NULL )alGenSources(1, &unit.sourceHandle);
    // Attach buffer 0 to source
    
    alSourcei(unit.sourceHandle, AL_BUFFER, unit.bufferHandle);
    
    if ((error = alGetError()) != AL_NO_ERROR)
    {
    }
    
    
    alSourcei(unit.sourceHandle, AL_LOOPING,  (unit.looping)?AL_TRUE:AL_FALSE);
    alSourcef(unit.sourceHandle, AL_PITCH, unit.pitch);
    alSourcef(unit.sourceHandle, AL_GAIN, unit.gain);
    
    alSourcePlay(unit.sourceHandle);
    

    
       // Exit

}
