#include "BattleLayers\MainLayer.h"

USING_NS_CC;

bool BattleMainLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//add backgorund
	this->setContentSize(areaSize);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			auto backgr = Sprite::create("background.png");
			backgr->setNormalizedPosition(Vec2(0.25f + 0.5f * i, 0.25f + 0.5f * j));
			this->addChild(backgr, -1);
		}
	}

	this->setPosition(Vec2(0, 0));//from 0,0 to -1920,-1080

	return true;
}

void BattleMainLayer::updateCamera()
{
	Vec2 center = *m_PlayerCenter;
	center *= -1;
	center += Vec2(960, 540);
	center.clamp(Vec2(-1920, -1080), Vec2(0, 0));
	this->setPosition(center);
}
