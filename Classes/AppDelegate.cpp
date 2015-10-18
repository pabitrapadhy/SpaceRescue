/**
 *	@programmer : Pabitra Padhy
 *
 *	@Game : Lifvator
 *	@Game Engine : cocos2d-x 3.6
 *	@Studio : Juego Studio Pvt. Ltd.
 */

#include "AppDelegate.h"
#include "LVSplashScene.h"

#include <vector>
#include <string>

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

static cocos2d::Size workingArea;
static float scaleFactor;
using namespace std;

USING_NS_CC;

typedef struct tagResource
{
    cocos2d::Size size;
    char directory[100];
}Resource;

static cocos2d::Size designResolutionSize;

static AppDelegate *sDelegate;

AppDelegate::AppDelegate() {

    sDelegate=this;
}

AppDelegate* AppDelegate::sharedAppDelegate()
{
    return sDelegate;
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // set the design resolution
    this->changeDesignResolutionToPortraitMode();
    
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = SplashScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

Vec2 AppDelegate::CPP(cocos2d::Vec2 point)
{
    CCLOG("sca=%f",scaleFactor);
    if (scaleFactor >= 1.0)
        return point;
    else
    {
        Size visibleSize = CCDirector::getInstance()->getVisibleSize();
        float scaleFactor_visibleSizeToWorkingArea = visibleSize.width / workingArea.width;
        
        return Vec2(scaleFactor_visibleSizeToWorkingArea * point.x,
                    scaleFactor_visibleSizeToWorkingArea * point.y);
    }
}

void AppDelegate::changeDesignResolutionToPortraitMode()
{
    designResolutionSize = cocos2d::Size(768, 1212);
    Resource mediumResource =  { cocos2d::Size(768, 1212),  "IPAD"   };
    Resource largeResource  =  { cocos2d::Size(1536, 2424), "IPAD-HD" };
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    Size frameSize = glview->getFrameSize();
    CCLOG("framesize    456=%f:%f",frameSize.width,frameSize.height);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
#else
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    
    CCLOG("designResolutionSize=%f:%f",designResolutionSize.width,designResolutionSize.height);
#endif
    
    vector<string> searchPath;
    
    float genericDeviceHeight = (frameSize.width > frameSize.height) ? frameSize.width : frameSize.height;
    float genericDeviceWidth = (frameSize.width > frameSize.height) ? frameSize.height : frameSize.width;
    
    if (genericDeviceWidth > designResolutionSize.width || genericDeviceHeight > designResolutionSize.height)
    {
        scaleFactor = largeResource.size.height/designResolutionSize.height;
        searchPath.push_back(largeResource.directory);
        searchPath.push_back(mediumResource.directory);
        director->setContentScaleFactor(scaleFactor);
    }
    
    else
    {
        workingArea = Size(ceilf((1024.0f / 960.0f) * 640.0f),1024);
        
        float scaleFactor_deviceToResources = genericDeviceWidth / mediumResource.size.width;
        
        if(scaleFactor_deviceToResources * mediumResource.size.height < genericDeviceHeight)
            scaleFactor_deviceToResources = genericDeviceHeight / mediumResource.size.height;
        
        float scaleFactor_workingAreaToDevice = (scaleFactor_deviceToResources * workingArea.height) / genericDeviceHeight;
        
        if((scaleFactor_workingAreaToDevice * genericDeviceWidth) < (scaleFactor_deviceToResources * workingArea.width))
            scaleFactor_workingAreaToDevice = (scaleFactor_deviceToResources * workingArea.width) / genericDeviceWidth;
        
        scaleFactor = scaleFactor_workingAreaToDevice;
        searchPath.push_back(mediumResource.directory);
        director->setContentScaleFactor(scaleFactor);
        
        CCLOG("scaleFactor=%f",scaleFactor);
    }
    
    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}




