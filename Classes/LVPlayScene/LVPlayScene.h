/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "LVLiftBox.h"
#include "LVAstronautManager.h"

#include "cocos2d.h"

USING_NS_CC;

// forward declarations
class AstronautManager;

class PlayScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // Constructor & Destructor
    PlayScene();
    ~PlayScene();
    
    // Array
    __Array * _levelNumArray;
    
    // initialize game elements
    void initializeAcceleration();
    void initializeGameElements();
    void initializeBG();
    void initializeSpaceBuilding();
    void initializeLiftBox();
    void initializeAstronauts();
    void initializeLabels();
    void initializeSounds();
    void initializeFloorNumberLabels();

    // accelerometer support implementation
    void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);

    // variables
    int score = 0;
    int life = 3;

    Label * _life;
    Label * _lifeCount;
    Label * _score;
    Label * _scoreCount;
    
    Sprite * _bgSprite;
    Sprite * _starSprite;
    Sprite * _cloudSprite;
    Sprite * _spaceBuilding;
    Sprite * _tutSprite;
    
    // game element references
    LiftBox * _liftBox;
    AstronautManager * _astronautMgr;
    
    // implement the "static create()" method manually
    CREATE_FUNC(PlayScene);
};

#endif // __PLAY_SCENE_H__
