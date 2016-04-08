#include "Physics\BodyBase.h"

USING_NS_CC;

BodyBase::BodyBase(const Vec2& _p, const Vec2& _d, const float _v, const std::string _sn)
	: m_Position(_p), m_Direction(_d), m_MaxVelocity(_v), m_BodyLayer(Node::create()),
	m_Sprite(Sprite::create(_sn))
{
	const float rot = GetRotation(_d);
	m_Sprite->retain();
	m_BodyLayer->retain();
	m_Sprite->setRotation(rot);
	m_BodyLayer->addChild(m_Sprite);
}

BodyBase::BodyBase(const Vec2& _p, const Vec2& _d, const float _v, const Sprite * _s)
	: m_Position(_p), m_Direction(_d), m_MaxVelocity(_v), m_BodyLayer(Node::create()),
	m_Sprite(Sprite::createWithSpriteFrame(_s->getSpriteFrame()))
{
	const float rot = GetRotation(_d);
	m_Sprite->retain();
	m_BodyLayer->retain();
	m_Sprite->setRotation(rot);
	m_BodyLayer->addChild(m_Sprite);
}

BodyBase::~BodyBase()
{
	m_Sprite->removeFromParent();
	m_Sprite->release();
	m_BodyLayer->removeFromParent();
	m_BodyLayer->release();

}

void BodyBase::UpdateWithoutRotation(const float dt, const Vec2& min, const Vec2& max)
{
	m_Position += m_Direction * m_Velocity * dt;
	m_Position.clamp(min, max);
	m_BodyLayer->setPosition(m_Position);
}

void BodyBase::UpdateWithRotation(const float dt, const Vec2& min, const Vec2& max)
{
	m_Position += m_Direction * m_Velocity * dt;
	m_Position.clamp(min, max);
	m_BodyLayer->setPosition(m_Position);

	const float rot = GetRotation(m_Direction);
	m_Sprite->setRotation(rot);
}

bool BodyBase::Collision(const BodyBase& r) const
{
	auto rect = RectApplyAffineTransform(r.m_Sprite->getBoundingBox(), r.m_BodyLayer->getNodeToParentAffineTransform());
	return Collision(rect);
}

bool BodyBase::Collision(const cocos2d::Rect& r) const
{
	auto rect = RectApplyAffineTransform(m_Sprite->getBoundingBox(), m_BodyLayer->getNodeToParentAffineTransform());
	return rect.intersectsRect(r);
}

void BodyBase::SetParent(Node * parent, int zOrder)
{
	if (m_BodyLayer->getParent())
		m_BodyLayer->removeFromParent();
	parent->addChild(m_BodyLayer, zOrder);
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

void BodyBase::SetVelocity(const float _v)
{
	if (_v > 1.f)
		m_Velocity = m_MaxVelocity;
	else if (_v < 0.f)
		m_Velocity = 0.f;
	else
		m_Velocity = m_MaxVelocity * _v;
	return;
}
