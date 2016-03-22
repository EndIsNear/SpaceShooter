#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__
#include "cocos2d.h"

class BattleScene : public cocos2d::Layer
{
	cocos2d::Layer * m_Joystick;
	cocos2d::Layer * m_MainLayer;
	cocos2d::Layer * m_HUDLayer;
	cocos2d::Layer * m_SettingsLayer;
public:
	CREATE_FUNC(BattleScene);
	static cocos2d::Scene* createScene();
	virtual bool init();

	void resumeGame();
	void update(float dt);
};


#endif // __BATTLE_SCENE_H__
