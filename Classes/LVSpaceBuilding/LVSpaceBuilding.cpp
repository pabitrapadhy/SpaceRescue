/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "LVSpaceBuilding.h"
#include "LVPlayScene.h"

#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#define MAX_HEIGHT 6

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

SpaceBuilding* SpaceBuilding::create()
{
    SpaceBuilding *spacebuilding = new SpaceBuilding();
    
    if(spacebuilding && spacebuilding->init())
    {
        spacebuilding->autorelease();
        return spacebuilding;
    }
    CC_SAFE_DELETE(spacebuilding);
    return nullptr;
}

bool SpaceBuilding::init()
{
    //////////////////////////////
    // 1. super init first
    
    if (!Sprite::init())
    {
        // if object of the superclass is not created then return false
        return false;
    }
    
    // initialize touch for space building
    this->initializeTouch();
    
    
    return true;
}

#pragma mark - Touch Initialization
void SpaceBuilding::initializeTouch()
{
    // Touch Listener
    auto listener = EventListenerTouchOneByOne::create();
    
    // Touch States
    
    // touch began
    listener->onTouchBegan = [&] (Touch *touch, Event *event) {
        // touch began logic
        auto playScene = static_cast<PlayScene *>(this->getParent());
        auto lift = static_cast<LiftBox *>(this->getChildByName("liftbox"));

        if(!lift->_isLiftBusy && this->getBoundingBox().containsPoint(playScene->convertToNodeSpace(touch->getLocation())))
            return true;
        
        return false;
    };
    
    // touch ended
    listener->onTouchEnded = [&] (Touch *touch, Event *event) {
        // touch ended logic
        
        SimpleAudioEngine::getInstance()->playEffect("sounds/tap.mp3");

        CCLOG("touch previous is %f", touch->getLocation().y);
        
        auto lift = static_cast<LiftBox *>(this->getChildByName("liftbox"));

        int floor = this->computeFloorNumberFromTouch(touch);
        
        if(floor != lift->_currFloor)
            lift->moveToFloor(floor);
    };
    
    // touch cancelled
    listener->onTouchCancelled = [&] (Touch *touch, Event *event) {
        // touch cancelled logic
//        auto lift = static_cast<LiftBox *>(this->getChildByName("liftbox"));
//
//        int floor = this->computeFloorNumberFromTouch(touch);
//        
//        if(floor != lift->_currFloor)
//            lift->moveToFloor(floor);
    };
    
    // Event Dispatcher
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

#pragma mark - compute floor number from touch
int SpaceBuilding::computeFloorNumberFromTouch(Touch *touch)
{
    auto touchLoc = this->convertToNodeSpace(touch->getLocation());
    
    auto cSizeY = this->getContentSize().height/6;
    auto tLocY = touchLoc.y;//this->convertToNodeSpace(touchLoc).y;
    
    int floorNum = 0;
  
    // looping through all the floors to get touch
    for(int i = 0; i < MAX_HEIGHT; i++)
    {
        CCLOG("i =%d",i);
        if((tLocY >= cSizeY*i) && (tLocY < (cSizeY*(i+1))))
        {
            CCLOG("i = %d",i);
            floorNum = i;
        }
    }
    
    CCLOG("Floor number is %d __________", floorNum);
    return floorNum;
}


