#ifndef __HUD_LAYER_H__
#define __HUD_LAYER_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"

const unsigned FireButtonPressed = 1;

class HUDLayer : public cocos2d::Layer
{
	cocos2d::ui::LoadingBar * m_LifeBar;
	cocos2d::ui::LoadingBar * m_ShieldBar;
	cocos2d::ui::Button * m_SettingsButton;
	cocos2d::Sprite * m_SkillButtons[4];
	cocos2d::GLProgramState * m_CdnShaders[4];
	unsigned m_PressedButtons;
	bool m_SettingsPressed;
public:
	HUDLayer() { m_SettingsPressed = 0; }
	virtual bool init();
	CREATE_FUNC(HUDLayer);

	void resetButtons();
	void updateCooldowns();

	void setPlayerLife(float perc);
	void setPlayerShield(float perc);
	unsigned getPlayerPressButtons() const { return m_PressedButtons; }
	bool isSettingsPressed();

#ifdef _WIN32
	void InitKeyboard();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
#endif
};

#endif //__HUD_LAYER_H__