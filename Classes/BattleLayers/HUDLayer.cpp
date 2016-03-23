#include "BattleLayers\HUDLayer.h"
#include "cocostudio\CocoStudio.h"
#include "cocostudio\ActionTimeline\CSLoader.h"

USING_NS_CC;

bool HUDLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// add the life hud
	auto LifeBarLayer = CSLoader::createNode("HUD/HUD.csb");
	this->addChild(LifeBarLayer, 1);
	m_LifeBar = static_cast<ui::LoadingBar*>(LifeBarLayer->getChildByName("HealthBar"));
	m_ShieldBar = static_cast<ui::LoadingBar*>(LifeBarLayer->getChildByName("ShieldBar"));
	m_SettingsButton = static_cast<ui::Button*>(LifeBarLayer->getChildByName("SettingsButton"));
	m_SettingsButton->setPressedActionEnabled(true);
	m_SettingsButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			m_SettingsPressed = 1;
	});


	auto ContrilsLayer = CSLoader::createNode("Controls/Controls.csb");
	this->addChild(ContrilsLayer, 1);
	ui::Button * fireButton = static_cast<ui::Button*>(ContrilsLayer->getChildByName("MainSkill"));
	fireButton->setPressedActionEnabled(true);
	fireButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::BEGAN == type)
			m_PressedButtons = 1;
		else if (ui::Widget::TouchEventType::ENDED == type || ui::Widget::TouchEventType::CANCELED == type)
			m_PressedButtons = 0;
	});

	m_PressedButtons = 0;

	return true;
}

void HUDLayer::setPlayerLife(float perc)
{
	m_LifeBar->setPercent(perc);
}

void HUDLayer::setPlayerShield(float perc)
{
	m_ShieldBar->setPercent(perc);
}

void HUDLayer::setFirePressed(cocos2d::Ref* pSender)
{
	m_PressedButtons |= FireButtonPressed;
}

bool HUDLayer::isSettingsPressed()
{
	if (m_SettingsPressed)
	{
		m_SettingsPressed = false;
		return true;
	}
	return false;
}
