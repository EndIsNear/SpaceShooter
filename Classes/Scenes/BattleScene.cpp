#include "Scenes/BattleScene.h"
#include "BattleLayers\Joystick.h"
#include "BattleLayers\MainLayer.h"
#include "BattleLayers\HUDLayer.h"
#include "BattleLayers\SettingsLayer.h"
#include "BattleLayers\EndGameStats.h"
#include "Scenes/MainMenuScene.h"
#include "BattleManager.h"
USING_NS_CC;


Scene* BattleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = BattleScene::create();
	scene->addChild(layer);
	return scene;
}

bool BattleScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_Joystick = Joystick::create();
	this->addChild(m_Joystick, 10);

	m_HUDLayer = HUDLayer::create();
	this->addChild(m_HUDLayer, 10);

	m_SettingsLayer = SettingsLayer::create();
	this->addChild(m_SettingsLayer, 20);
	m_SettingsLayer->setVisible(false);

	m_endStats = EndGameStats::create();
	this->addChild(m_endStats, 20);
	m_endStats->setVisible(false);

	m_MainLayer = BattleMainLayer::create();
	this->addChild(m_MainLayer, 0);
	this->scheduleUpdate();

	auto bm = BattleManager::Instance();
	bm->initialize();
	bm->SetParent(m_MainLayer);
	bm->setPlayerDirCallback(std::bind(&Joystick::getVelocity, reinterpret_cast<Joystick*>(m_Joystick)));
	bm->setPlayerButtonCallback(std::bind(&HUDLayer::getPlayerPressButtons, reinterpret_cast<HUDLayer*>(m_HUDLayer)));
	bm->setPlayerLifeDispCallback(std::bind(&HUDLayer::setPlayerLife, reinterpret_cast<HUDLayer*>(m_HUDLayer), std::placeholders::_1));
	bm->setPlayerShieldDispCallback(std::bind(&HUDLayer::setPlayerShield, reinterpret_cast<HUDLayer*>(m_HUDLayer), std::placeholders::_1));
	static_cast<BattleMainLayer*>(m_MainLayer)->setPlayerCenter(&(bm->ptrShip()->GetPositionRef()));
	
	return true;
}

void BattleScene::update(float dt)
{
	auto bm = BattleManager::Instance();
	//on death
	if(!bm->IsPlayerAlive() || !bm->IsThereEnemies())
	{
		this->unscheduleUpdate();
		//auto newScene = MainMenuScene::createScene();
		//BattleManager::Instance()->free();
		//Director::getInstance()->replaceScene(reinterpret_cast<Scene*>(newScene));
		m_endStats->setVisible(true);
		return;
	}

	//on settings pressed
	if (reinterpret_cast<HUDLayer*>(m_HUDLayer)->isSettingsPressed())
	{
		this->unscheduleUpdate();
		m_SettingsLayer->setVisible(true);
		reinterpret_cast<Joystick*>(m_Joystick)->resetJoystick();
		reinterpret_cast<HUDLayer*>(m_HUDLayer)->resetButtons();
	}

	bm->Update(dt);
	static_cast<BattleMainLayer*>(m_MainLayer)->updateCamera();
}

void BattleScene::resumeGame()
{
	this->scheduleUpdate();
	m_SettingsLayer->setVisible(false);
}
