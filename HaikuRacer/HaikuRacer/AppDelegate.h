#ifndef __AppDelegate_H__
#define __AppDelegate_H__

#include "OgrePlatform.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
#error This header is for use with Mac OS X only
#endif

#import <Cocoa/Cocoa.h>


// All this does is suppress some messages in the run log.  NSApplication does not
// implement buttonPressed and apps without a NIB have no target for the action.
@implementation NSApplication (_suppressUnimplementedActionWarning)
- (void) buttonPressed:(id)sender
{
    /* Do nothing */
}
@end


#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface AppDelegate : NSObject <NSApplicationDelegate>
#else
@interface AppDelegate : NSObject
#endif
{
    NSTimer *mTimer;
    HaikuRacerGame game;
    bool once;
    NSDate *mDate;
    double mLastFrameTime;
    double mStartTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (retain) NSTimer *mTimer;
@property (nonatomic) double mLastFrameTime;
@property (nonatomic) double mStartTime;

@end

#if __LP64__
static id mAppDelegate;
#endif

@implementation AppDelegate

@synthesize mTimer;
@dynamic mLastFrameTime;
@dynamic mStartTime;

- (double)mLastFrameTime
{
    return mLastFrameTime;
}

- (void)setLastFrameTime:(double)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        mLastFrameTime = frameInterval;
    }
}

- (void)go {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mStartTime = 0;
    mTimer = nil;
    once = false;
    try {
        game.startGame();
        
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        
        // Clear event times
		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }

    mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f) * mLastFrameTime
                                              target:self
                                            selector:@selector(renderOneFrame:)
                                            userInfo:nil
                                             repeats:YES];
    [pool release];
}

- (void)applicationDidFinishLaunching:(NSNotification *)application {
    mLastFrameTime = 1;
    mStartTime = 0;
    mTimer = nil;
    
    [self go];
}
- (void)renderOneFrame:(id)sender
{

    if(!BtOgreFramework::getSingletonPtr()->isOgreToBeShutDown() &&
       Ogre::Root::getSingletonPtr() && Ogre::Root::getSingleton().isInitialised())
    {
		if(BtOgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			            
            BtOgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			BtOgreFramework::getSingletonPtr()->m_pMouse->capture();

            if( BtOgreFramework::getSingletonPtr()->m_UpdatePhysics){
                if ( !once ){
                    BtOgreFramework::getSingletonPtr()->m_pTimer->reset();
                    mStartTime = BtOgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

                    once = true;
                }
                else{
                    mStartTime = BtOgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();
    
                    BtOgreFramework::getSingletonPtr()->updateOgre(mLastFrameTime);
                    BtOgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
                    game.updateGame();
                    mLastFrameTime = BtOgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - mStartTime;

                }
            }
            
            }
    }
    else
    {
        [mTimer invalidate];
        mTimer = nil;
        [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
    }
}

- (void)dealloc {
    if(mTimer)
    {
        [mTimer invalidate];
        mTimer = nil;
    }
    
    [super dealloc];
}

@end

#endif

