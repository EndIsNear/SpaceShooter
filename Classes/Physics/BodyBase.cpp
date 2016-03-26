#include "Physics\BodyBase.h"

USING_NS_CC;

BodyBase::BodyBase(const Vec2& _p, const Vec2& _d, const float _v, const std::string _sn)
	: m_Position(_p), m_Direction(_d), m_MaxVelocity(_v),
	m_Sprite(Sprite::create(_sn))
{
	const float rot = GetRotation(_d);
	m_Sprite->setRotation(rot);
}

BodyBase::BodyBase(const Vec2& _p, const Vec2& _d, const float _v, const Sprite * _s)
	: m_Position(_p), m_Direction(_d), m_MaxVelocity(_v),
	m_Sprite(Sprite::createWithSpriteFrame(_s->getSpriteFrame()))
{
	const float rot = GetRotation(_d);
	m_Sprite->setRotation(rot);
}

BodyBase::~BodyBase()
{
	m_Sprite->removeFromParent();
}

void BodyBase::UpdateWithoutRotation(const float dt, const Vec2& min, const Vec2& max)
{
	m_Position += m_Direction * m_Velocity * dt;
	m_Position.clamp(min, max);
	m_Sprite->setPosition(m_Position);
}

void BodyBase::UpdateWithRotation(const float dt, const Vec2& min, const Vec2& max)
{
	m_Position += m_Direction * m_Velocity * dt;
	m_Position.clamp(min, max);
	m_Sprite->setPosition(m_Position);

	const float rot = GetRotation(m_Direction);
	m_Sprite->setRotation(rot);
}

bool BodyBase::Collision(const BodyBase& r) const
{
	return Collision(r.m_Sprite->getBoundingBox());
}

bool BodyBase::Collision(const cocos2d::Rect& r) const
{
	return m_Sprite->getBoundingBox().intersectsRect(r);
}

void BodyBase::SetParent(Node * parent, int zOrder)
{
	if (m_Sprite->getParent())
		m_Sprite->removeFromParent();
	parent->addChild(m_Sprite, zOrder);
}

void BodyBase::SetDirection(const cocos2d::Vec2& _d)
{
	if (_d != Vec2::ZERO)
	{
		m_Velocity = m_MaxVelocity;
		m_Direction = _d;
	}
	else
		m_Velocity = 0;
}

