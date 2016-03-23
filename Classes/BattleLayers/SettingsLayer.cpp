#include "BattleLayers\SettingsLayer.h"

#include "Scenes\BattleScene.h"
#include "Scenes\MainMenuScene.h"
#include "BattleManager.h"

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

	auto quitButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Exit"));
	quitButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			this->onQuit();
	});

	return true;
}

void SettingsLayer::onQuit()
{
	auto newScene = MainMenuScene::createScene();
	BattleManager::Instance()->free();
	Director::getInstance()->replaceScene(reinterpret_cast<Scene*>(newScene));
}

void SettingsLayer::onResume()
{
	auto parent = this->getParent();
	reinterpret_cast<BattleScene*>(parent)->resumeGame();
}