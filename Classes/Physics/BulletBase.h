#ifndef  __BULLET_BASE_H__
#define  __BULLET_BASE_H__

#include "cocos2d.h"
#include "Physics\BodyBase.h"

class BulletBase : public BodyBase
{
public:
	BulletBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const std::string _sn)
		: BodyBase(_p, _d, _v, _sn)
	{
		m_Velocity = _v;
	}
	BulletBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const cocos2d::Sprite * _s)
		: BodyBase(_p, _d, _v, _s)
	{
		m_Velocity = _v;
	}

	virtual ~BulletBase() {}

	virtual bool Update(const float dt, bool * doOnExplosion = nullptr, const cocos2d::Vec2& min = cocos2d::Vec2::ZERO
		, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160))
	{
		BodyBase::UpdateWithoutRotation(dt, min, max);
		if (m_Position.x == min.x || m_Position.x == max.x)
			return false;
		if (m_Position.y == min.y || m_Position.y == max.y)
			return false;
		return true;
	}
};

class GranadeBullet : public BulletBase
{
	const float m_CastAfter;
	float m_TimeAfterCast;
public:
	GranadeBullet(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const std::string _sn, const float time = 100.f)
		: BulletBase(_p, _d, _v, _sn), m_CastAfter(time), m_TimeAfterCast(time)
	{
		m_Velocity = _v;
	}
	GranadeBullet(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const cocos2d::Sprite * _s, const float time = 100.f)
		: BulletBase(_p, _d, _v, _s), m_CastAfter(time), m_TimeAfterCast(time)
	{
		m_Velocity = _v;
	}

	virtual ~GranadeBullet() {}

	virtual bool Update(const float dt, bool * doOnExplosion = nullptr, const cocos2d::Vec2& min = cocos2d::Vec2::ZERO
		, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160))
	{
		m_TimeAfterCast -= dt;
		if (doOnExplosion && m_TimeAfterCast < 0.f)
		{
			m_TimeAfterCast = m_CastAfter;
			*doOnExplosion = true;
		}
		BodyBase::UpdateWithoutRotation(dt, min, max);
		if (m_Position.x == min.x || m_Position.x == max.x)
			return false;
		if (m_Position.y == min.y || m_Position.y == max.y)
			return false;
		return true;
	}
};


#endif // __BULLET_BASE_H__

