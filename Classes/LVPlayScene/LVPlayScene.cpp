/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "LVPlayScene.h"
#include "LVGameOverScene.h"
#include "LVSpaceBuilding.h"
#include "LVLiftBox.h"

#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"


#define MIN_FLOORS 6;


USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

PlayScene::PlayScene()
{
    
}

PlayScene::~PlayScene()
{
    this->_astronautMgr->release();
    this->_levelNumArray->release();
}

Scene* PlayScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PlayScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PlayScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //// INITIALIZATION
    this->_levelNumArray = __Array::create();
    this->_levelNumArray->retain();

    UserDefault::getInstance()->getIntegerForKey("myscore", 0);
    UserDefault::getInstance()->setIntegerForKey("myscore", score);
    
    this->initializeGameElements();

    
    return true;
}

#pragma mark - initialize game elements
void PlayScene::initializeGameElements()
{
    // initialize acceleration
    this->initializeAcceleration();

    // initialize BG
    this->initializeBG();
    
    // initialize spacebuilding
    this->initializeSpaceBuilding();
    
    // intialize liftbox
    this->initializeLiftBox();
    
    // initialize astronauts
    this->initializeAstronauts();
    
    // initialize labels
    this->initializeLabels();
    
    // initialize sounds
    this->initializeSounds();
   
    // initialize floor number labels
    this->initializeFloorNumberLabels();
    
}

#pragma mark - initialize acceleration
void PlayScene::initializeAcceleration()
{
    // enable acceleromater for splashscene
    Device::setAccelerometerEnabled(true);

    // set listener for accelerometer event
    auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(PlayScene::onAcceleration, this));

    // event dispatcher
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(accListener, this);

}

#pragma mark - initialize BG
void PlayScene::initializeBG()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // bg sprite
    this->_bgSprite = Sprite::create("LifvatorBG.png");
    this->_bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->_bgSprite->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y));
    this->addChild(this->_bgSprite, -1);


    // star sprite (accelerometer enabled)
    this->_starSprite = Sprite::create("star_bg.png");
    this->_starSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->_starSprite->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y));
    this->addChild(this->_starSprite, 1);

}

#pragma mark - initialize sounds
void PlayScene::initializeSounds()
{
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/GoingUp.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/GoingDown.mp3");
}

#pragma mark - initialize labels
void PlayScene::initializeLabels()
{
    // life label
    this->_life = Label::createWithTTF("Life: ", "fonts/space.ttf", 35);
    this->_life->setPosition(Vec2(this->getContentSize().width/6, this->getContentSize().height - 3*this->_life->getContentSize().height));
    this->addChild(this->_life);
    
    // life count label
    this->_lifeCount = Label::createWithTTF("3", "fonts/space.ttf", 35);
    this->_lifeCount->setPosition(this->getContentSize().width/6 + this->_life->getContentSize().width,
                                  this->getContentSize().height - 3*this->_life->getContentSize().height);
    this->addChild(this->_lifeCount);
    
    
    // score label
    this->_score = Label::createWithTTF("Score: ", "fonts/space.ttf", 35);
    this->_score->setPosition(Vec2(this->getContentSize().width/12*8, this->getContentSize().height - 3*this->_life->getContentSize().height));
    this->addChild(this->_score);
    
    // score count label
    this->_scoreCount = Label::createWithTTF("0", "fonts/space.ttf", 35);
    this->_scoreCount->setPosition(this->getContentSize().width/12*8 + this->_score->getContentSize().width,
                                   this->getContentSize().height - 3*this->_life->getContentSize().height);
    this->addChild(this->_scoreCount);
}

#pragma mark - intialize spacebuilding
void PlayScene::initializeSpaceBuilding()
{
    // SpaceBuilding
    this->_spaceBuilding = SpaceBuilding::create();
    this->_spaceBuilding->setTexture("SpaceStation.png");
    this->_spaceBuilding->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->_spaceBuilding->setPosition(Vec2(this->_bgSprite->getContentSize().width/2, this->_bgSprite->getContentSize().height/12));
    this->_bgSprite->addChild(this->_spaceBuilding, 2);

}

#pragma mark - intialize liftbox
void PlayScene::initializeLiftBox()
{
    // LiftBox
    this->_liftBox = LiftBox::create();
    this->_liftBox->setTexture("rocket4.png"); // textures can be set based on level also
    this->_liftBox->_textureTag = 4;
    this->_liftBox->setPosition(Vec2(this->_spaceBuilding->getContentSize().width/2, this->_liftBox->getContentSize().height/2));
    this->_liftBox->setName("liftbox");
    this->_spaceBuilding->addChild(this->_liftBox, 1);
    
    this->_liftBox->playScene = this;
    this->_liftBox->initializeTag();
    
    // TODO: set properties of lift based on game data
}

#pragma mark - initialize astronauts
void PlayScene::initializeAstronauts()
{
    // create astronaut manager
    this->_astronautMgr = AstronautManager::create();
    this->_astronautMgr->playScene = this;
    this->_astronautMgr->retain();

    this->_astronautMgr->createAstronauts();
    
}

#pragma mark - initialize floor number labels
void PlayScene::initializeFloorNumberLabels()
{
    // loop to MIN_FLOORS time and create labels, add them to _levelNumArray
    for(int i = 0; i < 6; i++)
    {
        std::stringstream ss;
        ss << "0" << i;
        
        auto levelNum = Label::createWithTTF(ss.str().c_str(), "fonts/space.ttf", 40);
        levelNum->setPosition(Vec2(this->_spaceBuilding->getContentSize().width/2,
                                   this->_spaceBuilding->getContentSize().height/6 * i + this->_spaceBuilding->getContentSize().height/12));
        levelNum->setOpacity(100);
        this->_spaceBuilding->addChild(levelNum);
        
        // add labels to _levelNumArray
        this->_levelNumArray->addObject(levelNum);
        
    }
}

#pragma mark - onAcceleration implementation code
void PlayScene::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
{
    CCLOG("x = %f", acc->x);
    CCLOG("y = %f", acc->y);
    // CCLOG("z = %f", acc->z);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // time between each frame
    float dt = 0.016f;

    // calculate buffer value
    Vec2 bufferVal = AppDelegate::CPP(Vec2(20, 7));

    // current sprite position
    auto currentPos = this->_bgSprite->getPosition();


    // change positions based on accelerometer input
    float newXBG = this->_bgSprite->getPositionX() + (80 * acc->x * dt);
    float newYBG = this->_bgSprite->getPositionY() + (100 * acc->y * dt);

    // set boundaries
    auto modifiedXBG = MIN(MAX(newXBG, origin.x + visibleSize.width/2 - bufferVal.x), origin.x + visibleSize.width/2 + bufferVal.x);
    auto modifiedYBG = MIN(MAX(newYBG, origin.y - bufferVal.y), origin.y + bufferVal.y);

    // set position
    this->_bgSprite->setPosition(Vec2(modifiedXBG, modifiedYBG));


     // change positions based on accelerometer input
    float newX = this->_starSprite->getPositionX() + (80 * (-acc->x) * dt);
    float newY = this->_starSprite->getPositionY() + (100 * (-acc->y) * dt);

     // set boundaries
    auto modifiedX = MIN(MAX(newX, origin.x + visibleSize.width/2 - bufferVal.x), origin.x + visibleSize.width/2 + bufferVal.x);
    auto modifiedY = MIN(MAX(newY, origin.y - bufferVal.y), origin.y + bufferVal.y);

    this->_starSprite->setPosition(Vec2(modifiedX, modifiedY));

}