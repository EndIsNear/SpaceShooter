#include "Joystick.h"
#include <algorithm>
USING_NS_CC;


bool Joystick::pointInJoystick(const Vec2& point)
{
	float dx = (point.x - m_Center.x);
	float dy = (point.y - m_Center.y);
	auto res = m_Radius * m_Radius >= dx * dx + dy * dy;
	return res;
}

bool Joystick::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_Velocity = Vec2::ZERO;
	m_Center = offset;
	Sprite *bg = Sprite::create("JoystickBG.png");
	bg->setPosition(m_Center);
	this->addChild(bg, 0);

	m_Thumb = Sprite::create("joystickThumb.png");
	m_Thumb->setPosition(m_Center);
	this->addChild(m_Thumb, 1);

	return true;
}

void Joystick::updateVelocity()
{
	Vec2 touchPos = m_Touch->getLocation();
	m_Velocity = touchPos - m_Center;
	const float len = m_Velocity.length();
	if (len > defradius)
	{
		m_Velocity *= (defradius / len);
	}
	m_Thumb->setPosition(m_Center + m_Velocity);

	m_Velocity = m_Velocity / defradius;
}

void Joystick::resetJoystick()
{
	m_Velocity = Vec2::ZERO;
	m_Thumb->setPosition(m_Center);
	m_Touch = nullptr;
}

void Joystick::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (m_Touch)
		return;
	for (auto& touch : touches)
	{
		if (pointInJoystick(touch->getLocation()))
		{
			m_Touch = touch;
			updateVelocity();
		}
	}
}

void Joystick::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if(m_Touch)
		updateVelocity();
}

void Joystick::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (m_Touch && std::find(touches.begin(), touches.end(), m_Touch) != touches.end()) {
		resetJoystick();
	}
}

void Joystick::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{
	if (m_Touch && std::find(touches.begin(), touches.end(), m_Touch) != touches.end()) {
		resetJoystick();
	}
}

void Joystick::enableTouches()
{
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(Layer::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Layer::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Layer::onTouchesEnded, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(Layer::onTouchesCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}
