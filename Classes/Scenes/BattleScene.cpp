#include "Scenes/BattleScene.h"
#include "Scenes/MainMenuScene.h"
#include "BattleLogic/BattleManager.h"
#include "GameLogic\GameManager.h"
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

	m_EndStatsLayer = EndGameStats::create();
	this->addChild(m_EndStatsLayer, 20);
	m_EndStatsLayer->setVisible(false);

	m_MainLayer = BattleMainLayer::create();
	this->addChild(m_MainLayer, 0);
	this->scheduleUpdate();

	auto bm = BattleManager::Instance();
	bm->initialize(new Spawner(GameManager::Instance()->GetCrnLevel()));
	bm->SetParent(m_MainLayer);
	setBattleManagerCallbacks();

	return true;
}

void BattleScene::update(float dt)
{
	auto bm = BattleManager::Instance();
	bm->Update(dt);

	//on death
	if(!bm->IsPlayerAlive() || !bm->IsThereEnemies())
	{
		this->unscheduleUpdate();
		if (!bm->IsPlayerAlive()) m_EndStatsLayer->SetStats("Lose!");
		else m_EndStatsLayer->SetStats("Win!");
		m_EndStatsLayer->SetTime(bm->GetElapsedTime());
		m_EndStatsLayer->setVisible(true);
		return;
	}

	//on settings pressed
	if (m_HUDLayer->isSettingsPressed())
	{
		this->unscheduleUpdate();
		m_SettingsLayer->setVisible(true);
	}

	m_MainLayer->updateCamera();
}

void BattleScene::restartGame()
{
	auto bm = BattleManager::Instance();
	bm->free();
	bm->initialize(new Spawner(GameManager::Instance()->GetCrnLevel()));
	bm->SetParent(m_MainLayer);
	setBattleManagerCallbacks();
}

void BattleScene::resumeGame()
{
	this->scheduleUpdate();
	m_Joystick->resetJoystick();
	m_HUDLayer->resetButtons();
	m_SettingsLayer->setVisible(false);
	m_EndStatsLayer->setVisible(false);
}

void BattleScene::setBattleManagerCallbacks()
{
	auto bm = BattleManager::Instance();
	bm->setPlayerDirCallback(std::bind(&Joystick::getVelocity, m_Joystick));
	bm->setPlayerButtonCallback(std::bind(&HUDLayer::getPlayerPressButtons, m_HUDLayer));
	bm->setPlayerLifeDispCallback(std::bind(&HUDLayer::setPlayerLife, m_HUDLayer, std::placeholders::_1));
	bm->setPlayerShieldDispCallback(std::bind(&HUDLayer::setPlayerShield, m_HUDLayer, std::placeholders::_1));
	m_MainLayer->setPlayerCenter(bm->ptrShip()->GetPositionRef());
}
