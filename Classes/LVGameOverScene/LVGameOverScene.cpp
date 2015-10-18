/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "LVGameOverScene.h"
#include "ui/CocosGUI.h"

#include "LVSplashScene.h"
#include "LVPlayScene.h"

#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
     // initialize acceleration
    this->initializeAcceleration();
    
    // bg sprite
    this->_bgSprite = Sprite::create("bg.png");
    this->_bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    this->_bgSprite->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y));
    this->addChild(this->_bgSprite);
    
    this->callGameOverPopup();
    
    
    return true;
}

#pragma mark - initialize acceleration
void GameOverScene::initializeAcceleration()
{
    // enable acceleromater for splashscene
    Device::setAccelerometerEnabled(true);

    // set listener for accelerometer event
    auto accListener = EventListenerAcceleration::create(CC_CALLBACK_2(GameOverScene::onAcceleration, this));

    // event dispatcher
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(accListener, this);

}

void GameOverScene::callGameOverPopup()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto congratzText = Label::createWithTTF("Mission Over !", "fonts/space.ttf", 100);
    congratzText->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/5*4));
    this->addChild(congratzText);

    
    std::stringstream ss_high;
    ss_high << "High Score: " << UserDefault::getInstance()->getIntegerForKey("highscore", 0);

    std::stringstream ss_myscore;
    ss_myscore << "Your Score: " << UserDefault::getInstance()->getIntegerForKey("myscore", 0);


    auto highScoreLabel = Label::createWithTTF(ss_high.str().c_str(), "fonts/space.ttf", 60);
    highScoreLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/3*2));
    this->addChild(highScoreLabel);

    auto yourScoreLabel = Label::createWithTTF(ss_myscore.str().c_str(), "fonts/space.ttf", 40);
    yourScoreLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/3*2 - yourScoreLabel->getContentSize().height*2));
    this->addChild(yourScoreLabel);

    // play btn
    auto homeBtn = Button::create("home_btn.png", "home_btn_hvr.png");
    homeBtn->addTouchEventListener([&] (Ref* sender, Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                SimpleAudioEngine::getInstance()->playEffect("sounds/click.mp3");
                auto scene = SplashScene::createScene();
                auto fadeout = TransitionFade::create(0.5f, scene);
                Director::getInstance()->replaceScene(fadeout);
                
                break;
            }
            default:
                break;
        }
    });
    homeBtn->setPosition(Vec2(origin.x + visibleSize.width/2 - homeBtn->getContentSize().width, origin.y + visibleSize.height/3));
    this->addChild(homeBtn);

    
    // play btn
    auto retryBtn = Button::create("retry_btn.png", "retry_btn_hvr.png");
    retryBtn->addTouchEventListener([&] (Ref* sender, Widget::TouchEventType type) {
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
    retryBtn->setPosition(Vec2(origin.x + visibleSize.width/2 + retryBtn->getContentSize().width, origin.y + visibleSize.height/3));
    this->addChild(retryBtn);

}

#pragma mark - onAcceleration implementation code
void GameOverScene::onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event)
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