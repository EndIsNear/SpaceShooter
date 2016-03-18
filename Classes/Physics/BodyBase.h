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

	

	bool Collision(const BodyBase& r);
	bool Collision(const cocos2d::Rect& r);

	//hmmm maybe add tag too
	void SetParent(cocos2d::Node * parent, int zOrder);
	
	void SetPosition(const cocos2d::Vec2& _p) { m_Position = _p; }
	void SetDirection(const cocos2d::Vec2& _d);
	void SetVelocity(const float _v) { m_Velocity = _v; }

	cocos2d::Vec2& GetPositionRef() { return m_Position; }
	cocos2d::Vec2 GetPosition() { return m_Position; }
	cocos2d::Vec2 GetDirection() { return m_Direction; }
	cocos2d::Sprite * GetSprite() { return m_Sprite; }
	float GetVelocity() { return m_Velocity; }


protected:
	void UpdateWithoutRotation(const cocos2d::Vec2& min = cocos2d::Vec2::ZERO, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160));
	void UpdateWithRotation(const cocos2d::Vec2& min = cocos2d::Vec2::ZERO, const cocos2d::Vec2& max = cocos2d::Vec2(3840, 2160));

	cocos2d::Vec2 m_Position;
	cocos2d::Vec2 m_Direction; ///< Should be with len 0-1
	cocos2d::Sprite * m_Sprite;
	float m_Velocity;
	float m_MaxVelocity;

};


#endif // __BODY_BASE_H__

