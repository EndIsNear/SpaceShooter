#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__
#include "cocos2d.h"

#include "BattleLayers\Joystick.h"
#include "BattleLayers\MainLayer.h"
#include "BattleLayers\HUDLayer.h"
#include "BattleLayers\SettingsLayer.h"
#include "BattleLayers\EndGameStats.h"

class BattleScene : public cocos2d::Layer
{
	Joystick        * m_Joystick;
	BattleMainLayer * m_MainLayer;
	HUDLayer        * m_HUDLayer;
	SettingsLayer   * m_SettingsLayer;
	EndGameStats    * m_EndStatsLayer;

	void setBattleManagerCallbacks();
public:
	CREATE_FUNC(BattleScene);
	static cocos2d::Scene* createScene();
	virtual bool init();

	void restartGame();
	void resumeGame();
	void update(float dt);
};


#endif // __BATTLE_SCENE_H__
