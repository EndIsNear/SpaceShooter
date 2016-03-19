#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"

const cocos2d::Vec2 offset(250.f, 250.f);
const float defradius = 125.f;

class Joystick : public cocos2d::Layer
{
public:
	Joystick() { m_Pressed = false; m_Radius = defradius; enableTouches(); }
	virtual bool init();
	cocos2d::Vec2 getVelocity() { return m_Velocity; }
	CREATE_FUNC(Joystick);

	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * unused_event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * unused_event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * unused_event);
	virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event * unused_event);
private:
	cocos2d::Vec2 m_Velocity;
	cocos2d::Vec2 m_Center;
	cocos2d::Sprite * m_Thumb;
	cocos2d::Touch * m_Touch;
	float m_Radius;
	bool m_Pressed;


	bool pointInJoystick(const cocos2d::Vec2& point);
	void updateVelocity();
	void resetJoystick();

	void enableTouches();
};

#endif //__JOYSTICK_H__