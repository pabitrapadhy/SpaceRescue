/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "LVLiftBox.h"
#include "LVSpaceBuilding.h"

#include "AppDelegate.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "LVPlayScene.h"

USING_NS_CC;

using namespace ui;
using namespace CocosDenshion;

LiftBox* LiftBox::create()
{
    LiftBox *liftbox = new LiftBox();
    
    if(liftbox && liftbox->init())
    {
        liftbox->autorelease();
        return liftbox;
    }
    CC_SAFE_DELETE(liftbox);
    return nullptr;
}

bool LiftBox::init()
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

#pragma mark - move to floor
void LiftBox::moveToFloor(int floorNum)
{
    // set the lift to busy
    this->_isLiftBusy = true;
    
    //    std::string liftMusicPath = (steps >= 0) ? "sounds/GoingUp.mp3" : "sounds/GoingDown.mp3";
    //    SimpleAudioEngine::getInstance()->playEffect(liftMusicPath.c_str());
    
    // mark the requested floor number label as red and opacity to full
    // get the label from the _levelNumArray
    auto requestedLevelLabel = static_cast<Label *>(this->playScene->_levelNumArray->getObjectAtIndex(floorNum));
    requestedLevelLabel->setTextColor(Color4B::RED);
    requestedLevelLabel->setOpacity(255);
    
    int steps = floorNum - this->_currFloor;
    
    CCLOG("Steps is %d_________", steps);
    
    // space building
    auto spaceBuilding = static_cast<SpaceBuilding *>(this->getParent());
    
    CCLOG("LIFT BUSY");
    
    // move action
    auto move = MoveBy::create(fabs(steps * 0.25f), Vec2(0, (/*spaceBuilding->getContentSize().height/6*/this->getContentSize().height * steps)));
    auto liftFreeCB = CallFunc::create( [&, floorNum, requestedLevelLabel] () {
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        // set the lift to free
        this->_isLiftBusy = false;
        CCLOG("LIFT FREE");
        
        // set the current floor number for the lift
        this->_currFloor = floorNum;
        
        // once the lift is free, set current level label color and opacity back to default
        requestedLevelLabel->setTextColor(Color4B::WHITE);
        requestedLevelLabel->setOpacity(100);
        
        // if the current floor is 0 that means the lift has reached the ground floor
        // now the astronauts will go out of the lift, so releasing astronauts from the shuttleArray
        if(this->_currFloor == 0)
        {
            // increase score
            this->playScene->score += (10 * this->_capacity);
            UserDefault::getInstance()->setIntegerForKey("myscore", this->playScene->score);

            
            std::stringstream ss;
            ss << (this->playScene->score);
            this->playScene->_scoreCount->setString(ss.str().c_str());

            // reset lift capacity
            this->_capacity = 0;
            
            // reset string of capacity label
            this->_capacityLabel->setString("0");
            
            // set all astronaut _inLift value to false, as they have reached the ground floor
            Ref *obj;
            CCARRAY_FOREACH(this->playScene->_astronautMgr->shuttleArray, obj)
            {
                auto astronaut = static_cast<Astronaut *>(obj);
                astronaut->_inLift = false;

                // lauch the rocket to the space
                this->launchRocket();

                // perform move action on these astronauts to move them outside of the visible area
                auto moveOutsideScreen = MoveBy::create(2.0f, Vec2(visibleSize.width/2 + 100, 0));
                auto removeAstroCB = CallFunc::create([&, astronaut] () {
                    // remove astronaut from shuttleArray
                    this->playScene->_astronautMgr->shuttleArray->removeObject(astronaut);
                    
                    // remove astronaut from the scene
                    astronaut->removeFromParentAndCleanup(true);
                });
                auto sequence = Sequence::create(moveOutsideScreen, removeAstroCB, nullptr);
                astronaut->runAction(sequence);
                
            }
            
            // release the shuttleArray
            this->playScene->_astronautMgr->shuttleArray->removeAllObjects();
        }
        
    });
    auto sequence = Sequence::create(move, liftFreeCB, nullptr);
    
    this->runAction(sequence);
    
    // perform same move action on all the astronauts in the lift
    Ref *obj;
    CCARRAY_FOREACH(this->playScene->_astronautMgr->shuttleArray, obj)
    {
        auto astronaut = static_cast<Astronaut *>(obj);
        
        // perform move action on astronauts
        auto moveAstronauts = MoveBy::create(fabs(steps * this->_liftSpeed), Vec2(0, (spaceBuilding->getContentSize().height/6 * steps)));
        astronaut->runAction(moveAstronauts);
        
    }
}

#pragma mark - add tag to show current capacity
void LiftBox::initializeTag()
{
    // add a tag image to show capacity
    auto tagSprite = Sprite::create("tag.png");
    tagSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    tagSprite->setPosition(this->getContentSize().width/2, this->getContentSize().height);
    this->addChild(tagSprite);
    
    // add a label to change the capacity
    this->_capacityLabel = Label::createWithTTF("0", "fonts/space.ttf", 40);
    this->_capacityLabel->setTextColor(Color4B::BLACK);
    this->_capacityLabel->setPosition(tagSprite->getContentSize().width/2, tagSprite->getContentSize().height/2);
    tagSprite->addChild(this->_capacityLabel);

}

#pragma mark - launch rocket to space
void LiftBox::launchRocket()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // get old texture of the rocket
    auto oldTexture = this->getTexture();

    // create new launch sprite
    auto launchRocketSprite = Sprite::createWithTexture(oldTexture);
    launchRocketSprite->setPosition(Vec2(this->playScene->_spaceBuilding->getContentSize().width/2, launchRocketSprite->getContentSize().height/2));
    this->playScene->_spaceBuilding->addChild(launchRocketSprite, 6);

    // perform action on the sprite
    auto moveUpForLaunch = MoveBy::create(1.0f, Vec2(0, visibleSize.height));
    auto easeLaunch = EaseOut::create(moveUpForLaunch, 0.5f);
    auto removeRocketCB = CallFunc::create([launchRocketSprite] () {

        launchRocketSprite->removeFromParentAndCleanup(true);
    });
    auto seq = Sequence::create(easeLaunch, removeRocketCB, nullptr);

    SimpleAudioEngine::getInstance()->playEffect("sounds/launch.mp3");

    // run action
    launchRocketSprite->runAction(seq);

    // set new texture for the rocket object
    auto randomValue = arc4random()%4 + 1;
    this->_textureTag = randomValue;

    std::stringstream ss;
    ss << "rocket" << this->_textureTag << ".png";

    this->setTexture(ss.str().c_str());
}
