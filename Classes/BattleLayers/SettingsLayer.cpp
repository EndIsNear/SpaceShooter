#include "BattleLayers\SettingsLayer.h"

#include "Scenes\BattleScene.h"
#include "Scenes\MainMenuScene.h"
#include "BattleLogic\BattleManager.h"

#include "cocostudio\CocoStudio.h"
#include "cocostudio\ActionTimeline\CSLoader.h"

USING_NS_CC;

bool SettingsLayer::init()
{
	if (!PopupBase::init()) return false;

	auto MainMenuLayer = CSLoader::createNode("InGameMenu/InGameMenu.csb");
	this->addChild(MainMenuLayer);

	auto resumeButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Resume"));
	resumeButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			this->onResume();
	});

	auto restartButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Restart"));
	restartButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			this->onRestart();
	});

	auto quitButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Exit"));
	quitButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			this->onQuit();
	});

	return true;
}

void SettingsLayer::onQuit()
{
	getParent()->unscheduleUpdate();
	auto newScene = MainMenuScene::createScene();
	BattleManager::Instance()->free();
	Director::getInstance()->popScene();
}

void SettingsLayer::onRestart()
{
	auto parent = this->getParent();
	reinterpret_cast<BattleScene*>(parent)->restartGame();
	reinterpret_cast<BattleScene*>(parent)->resumeGame();
}

void SettingsLayer::onResume()
{
	auto parent = this->getParent();
	reinterpret_cast<BattleScene*>(parent)->resumeGame();
}
