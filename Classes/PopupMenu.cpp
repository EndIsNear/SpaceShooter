#include "PopupMenu.h"

USING_NS_CC;

bool PopupBase::init()
{
	if (!LayerColor::initWithColor({ 0,0,0,122 })) return false;
	return true;
}

void PopupBase::setVisible(bool val)
{
	LayerColor::setVisible(val);
	auto parent = this->getParent();
	if (val)
	{
		Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(parent, true);
		Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, true);
	}
	else
	{
		Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(parent, true);
	}
}