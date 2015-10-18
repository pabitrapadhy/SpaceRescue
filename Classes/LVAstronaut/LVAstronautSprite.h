/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#ifndef __ASTRONAUT_H__
#define __ASTRONAUT_H__

#include "LVAstronautManager.h"

#include "cocos2d.h"

USING_NS_CC;

class Astronaut : public Sprite
{
public:
    // static create() method. create manually
    static Astronaut* create();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // variables
    int _level = 1;
    bool _inLift = false;
    

    
    // functions


};
#endif // __ASTRONAUT_H__