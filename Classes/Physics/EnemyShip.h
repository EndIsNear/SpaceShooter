#ifndef  __ENEMY_SHIP_H__
#define  __ENEMY_SHIP_H__

#include "cocos2d.h"
#include "ui\UILoadingBar.h"
#include "Physics\ShipBase.h"

class EnemyShip : public ShipBase
{
	cocos2d::ui::LoadingBar * m_HPBar;
	cocos2d::ui::LoadingBar * m_ShieldBar;
public:
	EnemyShip(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const std::string _sn)
		: ShipBase(_p, _d, _v, _sn)
	{
		m_Velocity = 0;
		m_HPBar = cocos2d::ui::LoadingBar::create("EnemyHealthBar.png", 100.f);
		m_ShieldBar = cocos2d::ui::LoadingBar::create("EnemyShieldBar.png", 100.f);
		m_BodyLayer->addChild(m_HPBar, -1);
		m_BodyLayer->addChild(m_ShieldBar, -1);
		m_HPBar->setPosition(cocos2d::Vec2(0, 75));
		m_ShieldBar->setPosition(cocos2d::Vec2(0, 78));
	}
	EnemyShip(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const cocos2d::Sprite * _s)
		: ShipBase(_p, _d, _v, _s)
	{
		m_Velocity = 0;
	}

	void Update(const float dt, const float lifeInPer, const float shieldInPer, const cocos2d::Vec2& min = cocos2d::Vec2::ZERO, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160))
	{
		BodyBase::UpdateWithRotation(dt, min, max);
		m_HPBar->setPercent(lifeInPer);
		m_ShieldBar->setPercent(shieldInPer);
	}
};


#endif // __ENEMY_SHIP_H__

