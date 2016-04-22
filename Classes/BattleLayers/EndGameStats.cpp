#include "BattleLayers\EndGameStats.h"

#include "Scenes\BattleScene.h"
#include "Scenes\MainMenuScene.h"
#include "BattleLogic\BattleManager.h"

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

	m_Result = Label::create();
	m_Result->initWithTTF("Win!", "MainMenu/Xcelsion Italic.ttf", 100);
	m_Result->setColor(Color3B(54, 202, 38));
	MainMenuLayer->addChild(m_Result, 0);
	m_Result->setNormalizedPosition(Vec2(0.5f, 0.5f));

	m_Time = Label::create();
	m_Time->initWithTTF("Time:", "MainMenu/Xcelsion Italic.ttf", 50);
	m_Time->setColor(Color3B(54, 202, 38));
	MainMenuLayer->addChild(m_Time, 0);
	m_Time->setNormalizedPosition(Vec2(0.5f, 0.35f));



	//m_Info += "WINs";
	return true;
}

void EndGameStats::SetStats(std::string stats)
{
	m_Result->setString(stats);
}

void EndGameStats::SetTime(const float time)
{
	const int sec = static_cast<int>(time);
	char tmp[1024];
	sprintf(tmp, "Time: %02d:%02d", sec / 60, sec % 60);
	m_Time->setString(tmp);
}

void EndGameStats::onContinue()
{
	getParent()->unscheduleUpdate();
	auto newScene = MainMenuScene::createScene();
	BattleManager::Instance()->free();
	Director::getInstance()->popScene();
}

void EndGameStats::onRestart()
{
	auto parent = this->getParent();
	reinterpret_cast<BattleScene*>(parent)->restartGame();
	reinterpret_cast<BattleScene*>(parent)->resumeGame();
}
