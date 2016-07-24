#ifndef __SKILL_CHOOSE_H__
#define __SKILL_CHOOSE_H__
#include "cocos2d.h"

class SkillChooseScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(SkillChooseScene);
	static cocos2d::Scene* createScene();
	virtual bool init();
};


#endif // __SKILL_CHOOSE_H__
