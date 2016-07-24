#include "Scenes/TempSkillChoose.h"

#include "cocostudio\CocoStudio.h"
#include "cocostudio\ActionTimeline\CSLoader.h"

USING_NS_CC;


Scene* SkillChooseScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SkillChooseScene::create();
	scene->addChild(layer);

	return scene;
}

bool SkillChooseScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto levelSelectLayer = CSLoader::createNode("Temp/TmpSkillChoose.csb");
	this->addChild(levelSelectLayer);


	return true;
}
