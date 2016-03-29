#include "PopupMenu.h"

USING_NS_CC;

bool PopupBase::init()
{
	if (!LayerColor::initWithColor({ 0,0,0,122 })) return false;


	auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(PopupBase::onTouch, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);



	return true;
}

bool PopupBase::onTouch(cocos2d::Touch*, cocos2d::Event*)
{
	return Layer::isVisible();
}
