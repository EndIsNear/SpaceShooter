#ifndef  __SHIP_BASE_H__
#define  __SHIP_BASE_H__

#include "cocos2d.h"
#include "Physics\BodyBase.h"

class ShipBase : public BodyBase
{
public:
	ShipBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const std::string _sn)
		: BodyBase(_p, _d, _v, _sn)
	{
		m_Velocity = 0;
	}
	ShipBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const cocos2d::Sprite * _s)
		: BodyBase(_p, _d, _v, _s)
	{
		m_Velocity = 0;
	}

	void Update(const float dt, const cocos2d::Vec2& min = cocos2d::Vec2::ZERO, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160))
	{
		BodyBase::UpdateWithRotation(dt, min, max);
	}
};


#endif // __SHIP_BASE_H__

