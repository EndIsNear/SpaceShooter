#include "AppDelegate.h"
#include "Utils.h"
#include "Scenes/MainMenuScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
#if DESKTOP_PLATFORM
static cocos2d::Size desktopResSize = cocos2d::Size(960, 540);
#endif

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::initGLView(cocos2d::Director * director)
{
	auto glview = director->getOpenGLView();
	if (!glview) {
#if DESKTOP_PLATFORM
		glview = GLViewImpl::createWithRect("idlerpgclient", Rect(0, 0, desktopResSize.width, desktopResSize.height));
#else
		glview = GLViewImpl::create("idlerpgclient");
#endif
		director->setOpenGLView(glview);
	}

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width,
		designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	return true;
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
	initGLView(director);

    register_all_packages();
	// turn on display FPS
	director->setDisplayStats(true);
	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60.f);
    // create a scene. it's an autorelease object
    auto scene = MainMenuScene::createScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

