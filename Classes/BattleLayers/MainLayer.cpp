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
	auto backgr = Sprite::create("background.jpg");
	backgr->setNormalizedPosition(Vec2(0.50f, 0.50f));
	this->addChild(backgr, -1);

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
