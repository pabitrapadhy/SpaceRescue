/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
 
USING_NS_CC;

class GameOverScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // game over dialog popup
    void callGameOverPopup();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);

    Sprite * _bgSprite;

    // functions

    // void initialize acceleration
    void initializeAcceleration();

    // accelerometer support implementation
    void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);

};

#endif // __GAME_OVER_SCENE_H__
