#ifndef __POPUP_MENU_H__
#define __POPUP_MENU_H__
#include "cocos2d.h"

class PopupBase : public cocos2d::LayerColor
{
public:
	virtual bool init();
	//override it to disable underling layer events
	virtual void setVisible(bool val) override;
	CREATE_FUNC(PopupBase);
};

#endif //__POPUP_MENU_H__