#ifndef  __LOGICAL_WEAPON_H__
#define  __LOGICAL_WEAPON_H__
#include "Utils\FileReader.h"
#include "BattleLogic\Skills\NormalAtack.h"

const size_t skillSize = 1;

enum UsedSkill : unsigned char
{
	None = 0,
	NormalAttack = (1 << 0),
	Skill1 = (1 << 1),
	Skill2 = (1 << 2),
	Skill3 = (1 << 3),
	Count
};

class LogicalWeapon
{
public:
	LogicalWeapon();
	LogicalWeapon(const LogicalWeapon& wpn);
	LogicalWeapon& operator=(const LogicalWeapon& wpn);

	void Update(const float dt)
	{
		for (size_t i = 0; i < skillSize; ++i)
		{
			m_Skills[i]->Update(dt);
		}
	}

	SkillResult Cast(UsedSkill usedSkill, cocos2d::Vec2& pos, cocos2d::Vec2& dir)
	{
		for (size_t i = 0; i < skillSize; ++i)
		{
			if (usedSkill & 1 << i && m_Skills[i]->CanCast()) return m_Skills[i]->OnCast(pos, dir);
		}
		return SkillResult();
	}
private:
	SkillInterface * m_Skills[skillSize];
};

#endif // __LOGICAL_WEAPON_H__
