#ifndef __BATTLE_MAIN_LAYER_H__
#define __BATTLE_MAIN_LAYER_H__

#include "cocos2d.h"
#include "Physics\BodyBase.h"
const cocos2d::Size areaSize(3840, 2160);

class BattleMainLayer : public cocos2d::Layer
{
public:
	BattleMainLayer() {}
	virtual bool init();
	CREATE_FUNC(BattleMainLayer);
	void updateCamera();

	void setPlayerCenter(cocos2d::Vec2 * _pl)///< used only to center the camera
	{
		m_PlayerCenter = _pl;
	}
private:
	cocos2d::Layer * backGround;
	cocos2d::Vec2 * m_PlayerCenter;
};

#endif //__BATTLE_MAIN_LAYER_H__