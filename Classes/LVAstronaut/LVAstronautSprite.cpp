/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "LVAstronautSprite.h"

USING_NS_CC;

Astronaut* Astronaut::create()
{
    Astronaut *astronaut = new Astronaut();
    
    if(astronaut && astronaut->init())
    {
        astronaut->autorelease();
        return astronaut;
    }
    CC_SAFE_DELETE(astronaut);
    return nullptr;
}

bool Astronaut::init()
{
    //////////////////////////////
    // 1. super init first
    
    if (!Sprite::init())
    {
        // if object of the superclass is not created then return false
        return false;
    }
    
    return true;
}