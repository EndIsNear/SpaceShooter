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
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//MenuItemFont::setFontSize(80);
	//MenuItemFont::setFontName("Marker Felt");

	//m_MenuItems.pushBack(MenuItemFont::create("PLAY!", CC_CALLBACK_1(MainMenuScene::menuBattleCallback, this)));
	//m_MenuItems.pushBack(MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenuScene::menuBackCallback, this)));

	//for (auto item : m_MenuItems)
	//	item->setColor(Color3B(255, 255, 255));

	//auto menu = Menu::createWithArray(m_MenuItems);
	//menu->setAnchorPoint(Vec2(0.5f, 0.5f));
	//menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//menu->alignItemsVerticallyWithPadding(20);
	//this->addChild(menu);

	auto MainMenuLayer = CSLoader::createNode("MainMenu/MainMenu.csb");
	this->addChild(MainMenuLayer);
	m_PlayButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Start"));
	m_PlayButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type)
			menuBattleCallback();
	});

	auto fireButton = static_cast<ui::Button*>(MainMenuLayer->getChildByName("Quit"));
	fireButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
		if (ui::Widget::TouchEventType::ENDED == type || ui::Widget::TouchEventType::CANCELED == type)
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
