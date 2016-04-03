#ifndef __LEVEL_SELECT_SCENE_H__
#define __LEVEL_SELECT_SCENE_H__
#include "cocos2d.h"
#include "cocos\base\CCVector.h"
#include "ui/UIButton.h"


class LevelSelectScene : public cocos2d::Layer
{
	cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
	cocos2d::ui::Button * m_PlayButton;
	//cocos2d::ui::Button m_PlayButton;
	cocos2d::ui::Button * m_ShipyardButton;
	cocos2d::ui::Button * m_SettingsButton;
	cocos2d::ui::Button * m_ExitButton;

public:
	CREATE_FUNC(LevelSelectScene);
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuStartCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void menuBackCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

};


#endif // __LEVEL_SELECT_SCENE_H__
