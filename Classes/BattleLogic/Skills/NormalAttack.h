#ifndef  __NORMAL_ATACK_H__
#define  __NORMAL_ATACK_H__

#include "BattleLogic\Skills\SkillBase.h"

class NormalAtack : public SkillInterface
{
	float m_Dmg;
	float m_BulletSpeeed;
	std::string m_SpriteName;

public:
	NormalAtack(const float dmg, const float cdn, const float bulletSpeed, const std::string& spriteName)
		: m_Dmg(dmg), m_BulletSpeeed(bulletSpeed), m_SpriteName(spriteName) 
	{
		m_MaxCooldown = cdn;
	}
	virtual SkillInterface * Clone() override { return new NormalAtack(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_SpriteName); };
	virtual ~NormalAtack() override {}

	/// Used when there is command to cast a skill
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override
	{
		SkillResult res;
		res.m_Source = this;
		res.m_Bullet = new BulletBase(pos, dir, m_BulletSpeeed, m_SpriteName);
		
		m_CrnCooldown = m_MaxCooldown;
		assert(res.m_Bullet != nullptr);
		return res;
	}

	/// Used after the projectile hit the enemy
	virtual SkillResult OnHit() override;
};

#endif // __NORMAL_ATACK_H__
