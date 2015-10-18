/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#ifndef __ASTRONAUT_MANAGER_H__
#define __ASTRONAUT_MANAGER_H__

#include "LVPlayScene.h"
#include "LVAstronautSprite.h"

#include "cocos2d.h"

USING_NS_CC;

// forward declaration
class PlayScene;
class Astronaut;

class AstronautManager : public Ref
{
public:
    // static create() method. create manually
    static AstronautManager* create();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // Constructor and Destructor
    AstronautManager();
    ~AstronautManager();
    
    
    // variables
    int _spawnedFloor = 1;
    
    // array
    __Array *astroArray;
    __Array *shuttleArray;
    
    // create astronauts
    void createAstronauts();
    
    // move towards lift
    void moveTowardsLift(Astronaut *astronaut, float xVal, float yVal);
    
    // calculate position
    Vec2 calculatePosition();
    
    // call game over
    void gameOver();
    
    // show miss puff image
    void showMissNotification(Astronaut *astronaut);
    
    // Reference to other classes
    PlayScene *playScene;
    
};

#endif // __ASTRONAUT_MANAGER_H__

