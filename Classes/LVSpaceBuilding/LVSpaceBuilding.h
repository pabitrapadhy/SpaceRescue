/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#ifndef __SPACE_BUILDING_H__
#define __SPACE_BUILDING_H__

#include "LVLiftBox.h"
#include "cocos2d.h"

USING_NS_CC;

class SpaceBuilding : public Sprite
{
public:
    // static create() method. create manually
    static SpaceBuilding* create();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // variables

    // initialize touch
    void initializeTouch();
    
    // compute floor no. from touch
    int computeFloorNumberFromTouch(Touch *touch);
    
};
#endif // __SPACE_BUILDING_H__