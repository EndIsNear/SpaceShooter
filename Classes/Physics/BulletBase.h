#ifndef  __BULLET_BASE_H__
#define  __BULLET_BASE_H__

#include "cocos2d.h"
#include "Physics\BodyBase.h"

class BulletBase : public BodyBase
{
	float m_lifetime;
public:
	BulletBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const std::string _sn, const float time = 100.f)
		: BodyBase(_p, _d, _v, _sn), m_lifetime(time)
	{
		m_Velocity = _v;
	}
	BulletBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const cocos2d::Sprite * _s)
		: BodyBase(_p, _d, _v, _s)
	{
		m_Velocity = _v;
	}

	virtual bool Update(const float dt, const cocos2d::Vec2& min = cocos2d::Vec2::ZERO, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160))
	{
		m_lifetime -= dt;
		if (m_lifetime < 0.f)
			return false;
		BodyBase::UpdateWithoutRotation(dt, min, max);
		if (m_Position.x == min.x || m_Position.x == max.x)
			return false;
		if (m_Position.y == min.y || m_Position.y == max.y)
			return false;
		return true;
	}
};

#endif // __BULLET_BASE_H__

