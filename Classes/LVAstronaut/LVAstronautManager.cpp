/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "LVAstronautManager.h"
#include "LVAstronautSprite.h"
#include "LVPlayScene.h"
#include "LVGameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;



#pragma mark - Constructor and Destructor
AstronautManager::AstronautManager()
{
    
}

AstronautManager::~AstronautManager()
{
    this->astroArray->release();
}

// static create method
AstronautManager* AstronautManager::create()
{
    AstronautManager *astronautMgr = new AstronautManager();
    
    if(astronautMgr && astronautMgr->init())
    {
        astronautMgr->autorelease();
        return astronautMgr;
    }
    CC_SAFE_DELETE(astronautMgr);
    return nullptr;
}

bool AstronautManager::init()
{
    // initialize the astro array
    this->astroArray = __Array::create();
    this->astroArray->retain();
    
    // initialize the shuttle array
    this->shuttleArray = __Array::create();
    this->shuttleArray->retain();
    
    return true;
}

void AstronautManager::createAstronauts()
{

    auto pos = this->calculatePosition();

    // create astronaut
    auto astronaut = Astronaut::create();
    astronaut->setTexture("astronaut.png");
    astronaut->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    astronaut->setPosition(Vec2(pos.x, pos.y));
    this->playScene->_spaceBuilding->addChild(astronaut);
    
    // set astronaut properties
    astronaut->_level = this->_spawnedFloor;
    
    // add astronaut to astroArray
    this->astroArray->addObject(astronaut);
    
    // move astronaut towards lift
    this->moveTowardsLift(astronaut, pos.x, pos.y);
    
    // action
    auto sequence = Sequence::create(DelayTime::create(2.5f), CallFunc::create([&] () {
        this->createAstronauts();
    }), nullptr);
    
    this->playScene->runAction(sequence);
    
}

void AstronautManager::moveTowardsLift(Astronaut *astronaut, float xVal, float yVal)
{
    // int val = (xVal < (this->playScene->getContentSize().width/2)) ? -1 : 1;
    
    auto moveSideways = MoveTo::create(3.5f, Vec2(this->playScene->_spaceBuilding->getContentSize().width/2
                                           /*+ astronaut->getContentSize().width*0.6*val*/, yVal));
    auto collisionCheckCB = CallFunc::create([&, astronaut] () {
        // collision logic
        __Array *temp = __Array::create();
        temp->retain();
        
        CCLOG("Astronaut's level is %d", astronaut->_level);
        CCLOG("Lift's current floor is %d", this->playScene->_liftBox->_currFloor);
        
        if(astronaut->getBoundingBox().intersectsRect(this->playScene->_liftBox->getBoundingBox())
           && (astronaut->_level == this->playScene->_liftBox->_currFloor) && (this->playScene->_liftBox->_capacity < 5)
           && (this->playScene->_liftBox->_isLiftBusy == false))
        {
            CCLOG("________INTERSECTS________");

            SimpleAudioEngine::getInstance()->playEffect("sounds/board.mp3");

            // perform a little moveup action on the sprite
            auto boardRocket = MoveBy::create(1.0f, Vec2(0, astronaut->getContentSize().height/3));
            auto disappearCB = CallFunc::create([astronaut] () {
                
                astronaut->setVisible(false);
            });
            astronaut->runAction(boardRocket);
            auto seq = Sequence::create(boardRocket, disappearCB, nullptr);

            // set the visibility of astronaut to false to make feel they abode the rocket.
            astronaut->setVisible(false);

            // increase the capacity count of the lift
            this->playScene->_liftBox->_capacity += 1;
            
            // update lift capacity label
            std::stringstream ss_capacity;
            ss_capacity << this->playScene->_liftBox->_capacity;
            this->playScene->_liftBox->_capacityLabel->setString(ss_capacity.str().c_str());
            
            // set flag of astronaut in lift
            astronaut->_inLift = true;
            
            // add them to the shuttleArray
            this->shuttleArray->addObject(astronaut);
            
        }
        else
        {
            SimpleAudioEngine::getInstance()->playEffect("sounds/oops.mp3");

            // decrease life count
            this->playScene->life -= 1;
            
            std::stringstream ss;
            ss << this->playScene->life;
            this->playScene->_lifeCount->setString(ss.str().c_str());
            
            astronaut->setVisible(false);
            
            // show miss notification
            this->showMissNotification(astronaut);
            
            // call game over
            if(this->playScene->life <= 0)
            {
                // add the high score if current score is greater than that
                auto highScore = UserDefault::getInstance()->getIntegerForKey("highscore", 0);

                if(this->playScene->score > highScore)
                    UserDefault::getInstance()->setIntegerForKey("highscore", this->playScene->score);
                this->gameOver();                
            }
            
            // add astronaut to temp array
            temp->addObject(astronaut);
            
        }
        
        // remove from temp array
        Ref *obj;
        
        CCARRAY_FOREACH(temp, obj)
        {
            // remove the astronaut from the astroArray
            astroArray->removeObject(obj);
            
            // remove the astronaut from the scene
            auto astronaut = static_cast<Astronaut *>(obj);
            astronaut->removeFromParentAndCleanup(true);
        }
        temp->release();
        
        
    });
    auto sequence = Sequence::create(moveSideways, collisionCheckCB, nullptr);
    astronaut->runAction(sequence);
    
}

Vec2 AstronautManager::calculatePosition()
{
    float height = this->playScene->_liftBox->getContentSize().height;//this->playScene->_spaceBuilding->getContentSize().height/6;
    float width = this->playScene->_spaceBuilding->getContentSize().width;
    
    int randomY = arc4random()%5 + 1; // astronauts will spawn in all floors except the ground floor
    int randomX = arc4random()%2;
    
    // save the spawned floor
    this->_spawnedFloor = randomY;
    
    float demoWidth = (randomX) ? width/5 : width/5*4;
    float demoHeight = randomY*height;
    
    return Vec2(demoWidth, demoHeight);
    
}

void AstronautManager::gameOver()
{
    auto scene = GameOverScene::createScene();
    auto fadeout = TransitionFade::create(0.5f, scene);
    Director::getInstance()->replaceScene(fadeout);
}

void AstronautManager::showMissNotification(Astronaut *astronaut)
{
    // get astronaut's position
    Vec2 pos = astronaut->getPosition();
    
    // miss sprite
    auto bubble = Sprite::create("miss_bubble.png");
    bubble->setPosition(Vec2(pos.x, pos.y + astronaut->getContentSize().height*1.5));
    this->playScene->_spaceBuilding->addChild(bubble, 5);
    
    // miss label
    auto misslabel = Label::createWithTTF("Oops!!", "fonts/space.ttf", 20);
    misslabel->setTextColor(Color4B::BLACK);
    misslabel->setPosition(Vec2(bubble->getContentSize().width/2, bubble->getContentSize().height/6*4));
    bubble->addChild(misslabel);
    
    // move up in 0.5 seconds and remove after that
    auto moveup = MoveBy::create(1.0f, Vec2(0, bubble->getContentSize().height/2));
    auto removeCB = CallFunc::create([bubble] () {
        bubble->removeFromParentAndCleanup(true);
    });
    auto seq = Sequence::create(moveup, removeCB, nullptr);
    
    bubble->runAction(seq);
}
