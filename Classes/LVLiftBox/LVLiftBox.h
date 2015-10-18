/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#ifndef __LIFT_BOX_H__
#define __LIFT_BOX_H__

#include "cocos2d.h"

USING_NS_CC;

class PlayScene;

class LiftBox : public Sprite
{
public:
    // static create() method. create manually
    static LiftBox* create();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // variables
    bool _isLiftBusy = false;
    int _currFloor = 0;
    int _capacity = 0;
    float _liftSpeed = 0.25f;   // this speed will be set when the object is created in the playscene
    int _floorDiffCount = 0;
    int _textureTag = 4;
    
    Label * _capacityLabel;
    
    // functios
    void moveToFloor(int floorNum);
    void initializeTag();
    void launchRocket();
    
    // reference
    PlayScene *playScene;
    
};
#endif // __LIFT_BOX_H__