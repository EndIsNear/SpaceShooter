#include "Scenes\LevelSelectScene.h"

#include "Scenes\MainMenuScene.h"
#include "Scenes\BattleScene.h"
#include "GameLogic\GameManager.h"

#include "cocostudio\CocoStudio.h"
#include "cocostudio\ActionTimeline\CSLoader.h"

USING_NS_CC;


Scene* LevelSelectScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LevelSelectScene::create();
	scene->addChild(layer);
	return scene;
}

bool LevelSelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto levelSelectLayer = CSLoader::createNode("LevelMenu/LevelSelectMenu.csb");
	this->addChild(levelSelectLayer);


	auto scrollView = levelSelectLayer->getChildByName("ScrollView");
	for (auto& child : scrollView->getChildren())
	{
		auto res = child->getName().find("Level");
		if (res != std::string::npos)
		{
			auto crnButton = reinterpret_cast<ui::Button*>(child);
			crnButton->addTouchEventListener(CC_CALLBACK_2(LevelSelectScene::menuStartCallback, this));
			auto subStr = child->getName().substr(child->getName().find_first_of("0123456789"));
			size_t idx = atoi(subStr.c_str());
			if (idx > GameManager::Instance()->GetLevelCount() - 1)
				crnButton->setEnabled(false);
		}
	}

	auto backButton = reinterpret_cast<ui::Button*>(levelSelectLayer->getChildByName("Back"));
	backButton->addTouchEventListener(CC_CALLBACK_2(LevelSelectScene::menuBackCallback, this));

	return true;
}

void LevelSelectScene::menuStartCallback(Ref* sender, ui::Widget::TouchEventType type)
{
	auto node = reinterpret_cast<Node*>(sender);
	if (ui::Widget::TouchEventType::ENDED == type)
	{
		auto subStr = node->getName().substr(node->getName().find_first_of("0123456789"));
		size_t idx = atoi(subStr.c_str());
		if (idx < GameManager::Instance()->GetLevelCount())
			GameManager::Instance()->SetCrnLevel(idx);
		auto newScene = BattleScene::createScene();
		Director::getInstance()->replaceScene(reinterpret_cast<Scene*>(newScene));
	}
}

void LevelSelectScene::menuBackCallback(Ref* sender, ui::Widget::TouchEventType type)
{
	if (ui::Widget::TouchEventType::ENDED == type)
	{
		auto newScene = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(reinterpret_cast<Scene*>(newScene));
	}
}
