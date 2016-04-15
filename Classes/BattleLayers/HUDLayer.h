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
	unsigned m_PressedButtons;
	bool m_SettingsPressed;
public:
	HUDLayer() { m_SettingsPressed = 0; }
	virtual bool init();
	CREATE_FUNC(HUDLayer);

	void resetButtons();

	void setPlayerLife(float perc);
	void setPlayerShield(float perc);
	unsigned getPlayerPressButtons() const { return m_PressedButtons; }
	bool isSettingsPressed();
};

#endif //__HUD_LAYER_H__