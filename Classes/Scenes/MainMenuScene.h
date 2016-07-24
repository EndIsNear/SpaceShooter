#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__
#include "cocos2d.h"
#include "cocos\base\CCVector.h"
#include "ui/UIButton.h"


class MainMenuScene : public cocos2d::Layer
{
	cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
	cocos2d::ui::Button * m_PlayButton;
	cocos2d::ui::Button * m_ShipyardButton;
	cocos2d::ui::Button * m_SettingsButton;
	cocos2d::ui::Button * m_ExitButton;

public:
	CREATE_FUNC(MainMenuScene);
	static cocos2d::Scene* createScene();
	virtual bool init();

	void menuBattleCallback();
	void menuShipyardCallback();
	void menuBackCallback();
};


#endif // __MAIN_MENU_SCENE_H__
