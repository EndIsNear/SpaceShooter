#ifndef  __BODY_BASE_H__
#define  __BODY_BASE_H__

#include "cocos2d.h"
#include "Utils.h"

class BodyBase
{
public:
	BodyBase() {}
	BodyBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const std::string _sn);
	BodyBase(const cocos2d::Vec2& _p, const cocos2d::Vec2& _d, const float _v, const cocos2d::Sprite * _s);

	~BodyBase();

	

	bool Collision(const BodyBase& r) const;
	bool Collision(const cocos2d::Rect& r) const;

	//hmmm maybe add tag too
	void SetParent(cocos2d::Node * parent, int zOrder);
	
	void SetPosition(const cocos2d::Vec2& _p) { m_Position = _p; }
	void SetDirection(const cocos2d::Vec2& _d);
	void SetVelocity(const float _v) { m_Velocity = _v < m_MaxVelocity ? _v : m_MaxVelocity; }

	const cocos2d::Vec2* GetPositionRef() const { return &m_Position; }
	cocos2d::Vec2 GetPosition() const { return m_Position; }
	cocos2d::Vec2 GetDirection() const { return m_Direction; }
	cocos2d::Sprite * GetSprite() const { return m_Sprite; }
	float GetVelocity() const { return m_Velocity; }
	float GetMaxVelocity() const { return m_MaxVelocity; }


protected:
	void UpdateWithoutRotation(const float dt, const cocos2d::Vec2& min = cocos2d::Vec2::ZERO, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160));
	void UpdateWithRotation(const float dt, const cocos2d::Vec2& min = cocos2d::Vec2::ZERO, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160));

	cocos2d::Vec2 m_Position;
	cocos2d::Vec2 m_Direction; ///< Should be with len 0-1
	cocos2d::Sprite * m_Sprite;
	float m_Velocity;
	float m_MaxVelocity;

};


#endif // __BODY_BASE_H__

