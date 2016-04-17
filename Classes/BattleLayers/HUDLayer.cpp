#include "BattleLayers\HUDLayer.h"
#include "BattleLogic\BattleManager.h"

#include "Shaders\Shaders.h"

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
		m_SkillButtons[i] = static_cast<Sprite*>(ContrilsLayer->getChildByName(namePrefix + tmp));
		const float radius = m_SkillButtons[i]->getContentSize().width / 2;
		const Vec2 center = m_SkillButtons[i]->getPosition();

		auto listener = cocos2d::EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [this, i, radius, center](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			Vec2 pnt = touch->getLocation();
			if ((pnt - center).lengthSquared() < radius * radius)
			{
				m_PressedButtons |= (1 << i);
				return true;
			}
			return false;
		};

		listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
		{
			m_PressedButtons &= ~(1 << i);
		};
		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_SkillButtons[i]);

		auto program = GetCooldownShader();
		m_CdnShaders[i] = GLProgramState::create(program);
		m_SkillButtons[i]->setGLProgram(program);
		m_SkillButtons[i]->setGLProgramState(m_CdnShaders[i]);
		UpdateCooldownShader(m_CdnShaders[i], 1.f);
	}

	m_PressedButtons = 0;

#ifdef _WIN32
	InitKeyboard();
#endif

	return true;
}

void HUDLayer::resetButtons()
{
	m_PressedButtons = 0;
}

void HUDLayer::updateCooldowns()
{
	float cdns[4];
	//if there is cooldown higher than 0 (in percents)
	if (BattleManager::Instance()->GetPlayerCooldowns(cdns))
	{
		for (size_t i = 0; i < 4; ++i)
		{
			//now update the given shander
			UpdateCooldownShader(m_CdnShaders[i], 1.f - cdns[i]);
		}
	}
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

#ifdef _WIN32
void HUDLayer::InitKeyboard()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HUDLayer::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(HUDLayer::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HUDLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	for (int i = 0; i < 4; ++i)
	{
		if (static_cast<int>(keyCode) == static_cast<int>(EventKeyboard::KeyCode::KEY_1) + i)
		{
			m_PressedButtons |= (1 << i);
		}
	}
}

void HUDLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	for (int i = 0; i < 4; ++i)
	{
		if (static_cast<int>(keyCode) == static_cast<int>(EventKeyboard::KeyCode::KEY_1) + i)
		{
			m_PressedButtons &= ~(1 << i);
		}
	}
}

#endif
