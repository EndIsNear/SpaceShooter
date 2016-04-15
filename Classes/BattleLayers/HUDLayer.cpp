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
	
	const std::string namePrefix("Skill");
	for (unsigned i = 0; i < 4; ++i)
	{
		char tmp[16];
		sprintf(tmp, "%d", i);
		auto fireButton = static_cast<ui::Button*>(ContrilsLayer->getChildByName(namePrefix + tmp));
		fireButton->setPressedActionEnabled(true);
		fireButton->addTouchEventListener([this, i](Ref* sender, ui::Widget::TouchEventType type) {
			if (ui::Widget::TouchEventType::BEGAN == type)
				m_PressedButtons |= (1 << i);
			else if (ui::Widget::TouchEventType::ENDED == type || ui::Widget::TouchEventType::CANCELED == type)
			 	m_PressedButtons &= ~(1 << i);
		});
	}

	m_PressedButtons = 0;

	return true;
}

void HUDLayer::resetButtons()
{
	m_PressedButtons = 0;
}

void HUDLayer::setPlayerLife(float perc)
{
	m_LifeBar->setPercent(perc);
}

void HUDLayer::setPlayerShield(float perc)
{
	m_ShieldBar->setPercent(perc);
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
