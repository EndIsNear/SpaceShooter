#include "BattleLayers\EndGameStats.h"

#include "Scenes\BattleScene.h"
#include "Scenes\MainMenuScene.h"
#include "BattleManager.h"

#include "cocostudio\CocoStudio.h"
#include "cocostudio\ActionTimeline\CSLoader.h"

USING_NS_CC;

bool EndGameStats::init()
{
	if (!PopupBase::init()) return false;

	auto MainMenuLayer = CSLoader::createNode("EndGameMenu/EndGameMenu.csb");
	this->addChild(MainMenuLayer);

	auto resumeButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Continue"));
	resumeButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			this->onContinue();
	});

	auto restartButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Restart"));
	restartButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			this->onRestart();
	});

	return true;
}

void EndGameStats::onContinue()
{
	auto newScene = MainMenuScene::createScene();
	BattleManager::Instance()->free();
	Director::getInstance()->replaceScene(reinterpret_cast<Scene*>(newScene));
}

void EndGameStats::onRestart()
{
	auto parent = this->getParent();
	reinterpret_cast<BattleScene*>(parent)->restartGame();
	reinterpret_cast<BattleScene*>(parent)->resumeGame();
}
