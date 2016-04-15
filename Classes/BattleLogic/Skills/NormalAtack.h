#ifndef  __NORMAL_ATACK_H__
#define  __NORMAL_ATACK_H__

#include "BattleLogic\Skills\SkillBase.h"

class NormalAtack : public SkillInterface
{
	float dmg;
	float bulletSpeeed;
	std::string spriteName;

public:
	NormalAtack() { dmg = 100.f; bulletSpeeed = 800.f; spriteName = "bullet.png"; }
	virtual SkillInterface * Clone() { return new NormalAtack(); };
	virtual ~NormalAtack() {}

	/// Used when there is command to cast a skill
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir)
	{
		SkillResult res;
		res.m_Source = this;
		res.m_Bullet = new BulletBase(pos, dir, bulletSpeeed, spriteName);
		
		m_CrnCooldown = m_MaxCooldown;
		assert(res.m_Bullet != nullptr);
		return res;
	}

	/// Used after the projectile hit the enemy
	virtual SkillResult OnHit()
	{
		return SkillResult();
	}
};

#endif // __NORMAL_ATACK_H__
