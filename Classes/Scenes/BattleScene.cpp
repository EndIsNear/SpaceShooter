#include "Scenes/BattleScene.h"
#include "BattleLayers\Joystick.h"
#include "BattleLayers\MainLayer.h"
#include "BattleLayers\HUDLayer.h"
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

	m_MainLayer = BattleMainLayer::create();
	this->addChild(m_MainLayer, 0);
	this->scheduleUpdate();

	auto bm = BattleManager::Instance();
	bm->SetParent(m_MainLayer);
	bm->setPlayerDirCallback(std::bind(&Joystick::getVelocity, reinterpret_cast<Joystick*>(m_Joystick)));
	bm->setPlayerButtonCallback(std::bind(&HUDLayer::getPlayerPressButtons, reinterpret_cast<HUDLayer*>(m_HUDLayer)));
	bm->setPlayerLifeDispCallback(std::bind(&HUDLayer::setPlayerLife, reinterpret_cast<HUDLayer*>(m_HUDLayer), std::placeholders::_1));
	static_cast<BattleMainLayer*>(m_MainLayer)->setPlayerCenter(&(bm->ptrShip()->GetPositionRef()));
	
	return true;
}

void BattleScene::update(float dt)
{
	//check for quit
	if (reinterpret_cast<HUDLayer*>(m_HUDLayer)->isSettingsPressed())
	{
		this->unscheduleUpdate();
		auto newScene = MainMenuScene::createScene();
		BattleManager::ReleaseInstance();
		Director::getInstance()->replaceScene(reinterpret_cast<Scene*>(newScene));
		return;
	}

	BattleManager::Instance()->update();
	static_cast<BattleMainLayer*>(m_MainLayer)->updateCamera();
}
