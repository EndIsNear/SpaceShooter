#ifndef  __LOGICAL_WEAPON_H__
#define  __LOGICAL_WEAPON_H__
#include "Utils\FileReader.h"
#include "BattleLogic\Skills\DamageSkills.h"
#include "BattleLogic\Skills\BuffSkills.h"

const size_t skillSize = 4;

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
	LogicalWeapon(std::unique_ptr<SkillInterface>& skill0, std::unique_ptr<SkillInterface>& skill1,
		std::unique_ptr<SkillInterface>& skill2, std::unique_ptr<SkillInterface>& skill3);
	LogicalWeapon(const LogicalWeapon& wpn);
	LogicalWeapon& operator=(const LogicalWeapon& wpn);

	void Update(const float dt)
	{
		for (size_t i = 0; i < skillSize; ++i)
		{
			m_Skills[i]->Update(dt);
		}
	}

	SkillResult Cast(UsedSkill usedSkill, const cocos2d::Vec2& pos, const cocos2d::Vec2& dir)
	{
		for (size_t i = 0; i < skillSize; ++i)
		{
			if (usedSkill & 1 << i && m_Skills[i]->CanCast()) 
				return m_Skills[i]->OnCast(pos, dir);
		}
		return SkillResult();
	}

	//SkillInterface * GetSkillAt(size_t idx)
	//{
	//	assert(idx <= skillSize);
	//	assert(m_Skills[idx] != nullptr);
	//	return m_Skills[idx];
	//}

	float GetCooldownAt(size_t idx)
	{
		assert(idx <= skillSize);
		assert(m_Skills[idx] != nullptr);
		return m_Skills[idx]->GetCrnCooldown();
	}

	void SetCooldownAt(size_t idx, float cdn)
	{
		assert(idx <= skillSize);
		assert(m_Skills[idx] != nullptr);
		m_Skills[idx]->SetCrnCooldown(cdn);
	}

	float GetMaxCooldownAt(size_t idx)
	{
		assert(idx <= skillSize);
		assert(m_Skills[idx] != nullptr);
		return m_Skills[idx]->GetMaxCooldown();
	}

	void SetMaxCooldownAt(size_t idx, float cdn)
	{
		assert(idx <= skillSize);
		assert(m_Skills[idx] != nullptr);
		m_Skills[idx]->SetMaxCooldown(cdn);
	}

	float GetPrcCooldownAt(size_t idx)
	{
		assert(idx <= skillSize);
		assert(m_Skills[idx] != nullptr);
		return m_Skills[idx]->GetCooldownInPerc();
	}
private:
	std::unique_ptr<SkillInterface> m_Skills[skillSize];
};

#endif // __LOGICAL_WEAPON_H__
