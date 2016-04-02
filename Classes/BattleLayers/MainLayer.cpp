#include "BattleLayers\MainLayer.h"

USING_NS_CC;

bool BattleMainLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_Backgroud = Layer::create();
	m_Backgroud->setContentSize(areaSize);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			auto bg = Sprite::create("background.png");
			bg->setNormalizedPosition(Vec2(0.33f + 0.33f * i, 0.33f + 0.33f * j));
			m_Backgroud->addChild(bg, -1);
		}
	}
	this->addChild(m_Backgroud, -5);


	//add backgorund
	this->setContentSize(areaSize);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			auto foreground = Sprite::create("foreground.png");
			foreground->setNormalizedPosition(Vec2(0.25f + 0.5f * i, 0.25f + 0.5f * j));
			this->addChild(foreground, -1);
		}
	}


	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->setPosition(Vec2(0, 0));//from 0,0 to -1920,-1080

	return true;
}

void BattleMainLayer::updateCamera()
{
	const Vec2 idealCenter(1920.f, 1080.f);
	Vec2 offset = idealCenter - *m_PlayerCenter;
	offset.clamp(-0.5f * idealCenter, 0.5f * idealCenter);

	this->setPosition(offset - idealCenter / 2);
	//little hack because backgroud is relative to this layer
	m_Backgroud->setPosition((offset - idealCenter) * 0.33f);
}
