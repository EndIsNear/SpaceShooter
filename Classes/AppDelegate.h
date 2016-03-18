#ifndef  __APP_DELEGATE_H__
#define  __APP_DELEGATE_H__

#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application
{
	bool initGLView(cocos2d::Director * director);
public:
	AppDelegate() {}
	virtual ~AppDelegate() {}

    virtual void initGLContextAttrs();
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
};

#endif // __APP_DELEGATE_H__

