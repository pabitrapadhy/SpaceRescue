/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "AppDelegate.h"
#include "LVSplashScene.h"
#include "LVPlayScene.h"

#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    UserDefault::getInstance()->getIntegerForKey("highscore", 0);
    UserDefault::getInstance()->getIntegerForKey("myscore", 0);

    SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/BGSound.mp3", true);

    // initialize acceleration
    this->initializeAcceleration();

    // bg sprite
    this->_bgSprite = Sprite::create("bg.png");
    this->_bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->_bgSprite->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y));
    this->addChild(this->_bgSprite);

    // game name level
    std::string name = "SPACE\nRESCUE";
    auto label = Label::createWithTTF(name.c_str(), "fonts/space.ttf", 100, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setTextColor(Color4B(255,243,135,255));
    label->enableOutline(Color4B::BLACK, 3);
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/3*2));
    this->addChild(label, 1);
    
    // play btn
    auto playBtn = Button::create("play.png", "play.png", "play.png");
    playBtn->addTouchEventListener([&] (Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                SimpleAudioEngine::getInstance()->playEffect("sounds/click.mp3");
                auto scene = PlayScene::createScene();
                auto fadeout = TransitionFade::create(0.5f, scene);
                Director::getInstance()->replaceScene(fadeout);
                
                break;
            }
            default:
                break;
        }
    });
    playBtn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/3));
    this->addChild(playBtn);

    
    return true;
}

#pragma mark - initialize acceleration
void SplashScene::initializeAcceleration()
{
    // enable acceleromater for splashscene
    Device::setAccelerometerEnabled(true);

    // set listener for accelerometer event
    auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(SplashScene::onAcceleration, this));

    // event dispatcher
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(accListener, this);

}

#pragma mark - onAcceleration implementation code
void SplashScene::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
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
    float newX = this->_bgSprite->getPositionX() + (80 * acc->x * dt);
    float newY = this->_bgSprite->getPositionY() + (100 * acc->y * dt);

    // set boundaries
    auto modifiedX = MIN(MAX(newX, origin.x + visibleSize.width/2 - bufferVal.x), origin.x + visibleSize.width/2 + bufferVal.x);
    auto modifiedY = MIN(MAX(newY, origin.y - bufferVal.y), origin.y + bufferVal.y);

    // set position
    this->_bgSprite->setPosition(Vec2(modifiedX, modifiedY));

}