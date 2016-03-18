#include "Scenes/MainMenuScene.h"
#include "Scenes/BattleScene.h"
#include "cocostudio\CocoStudio.h"
#include "cocostudio\ActionTimeline\CSLoader.h"

USING_NS_CC;


Scene* MainMenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto MainMenuLayer = CSLoader::createNode("MainMenu/MainMenu.csb");
	this->addChild(MainMenuLayer);
	m_PlayButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Start"));
	m_PlayButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			menuBattleCallback();
	});

	m_ShipyardButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Shipyard"));
	m_ShipyardButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			menuBackCallback();
	});

	m_SettingsButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Settings"));
	m_SettingsButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			menuBackCallback();
	});

	auto fireButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Quit"));
	fireButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			menuBackCallback();
	});

	return true;
}

void MainMenuScene::menuBattleCallback()
{
	auto newScene = BattleScene::createScene();
	Director::getInstance()->replaceScene(reinterpret_cast<Scene*>(newScene));
}

void MainMenuScene::menuBackCallback()
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
