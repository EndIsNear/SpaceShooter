#ifndef  __DAMAGE_SKILLS_H__
#define  __DAMAGE_SKILLS_H__

#include "BattleLogic\Skills\SkillBase.h"

///////////////////////////////////////
// Normal Attack
///////////////////////////////////////

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
		res.m_Bullets.push_back(new BulletBase(pos, dir.getNormalized(), m_BulletSpeeed, m_SpriteName));

		m_CrnCooldown = m_MaxCooldown;
		res.m_Type = SkillResult::ResultType::Bullet;
		return res;
	}

	/// Used after the projectile hit the enemy
	virtual SkillResult OnHit() override;
};

///////////////////////////////////////
// DoT Attack
///////////////////////////////////////

class DoTAttack : public SkillInterface
{
	float m_Dmg;//this dmg is dmg per second
	float m_BulletSpeeed;
	float m_EffectTime;
	std::string m_SpriteName;
public:
	DoTAttack(const float dmg, const float cdn, const float bulletSpeed, const std::string& spriteName, const float effTime)
		: m_Dmg(dmg), m_BulletSpeeed(bulletSpeed), m_SpriteName(spriteName), m_EffectTime(effTime)
	{
		m_MaxCooldown = cdn;
	}
	virtual SkillInterface * Clone() override { return new DoTAttack(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_SpriteName, m_EffectTime); };
	virtual ~DoTAttack() override {}

	/// Used when there is command to cast a skill
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override
	{
		SkillResult res;
		res.m_Source = this;
		res.m_Bullets.push_back(new BulletBase(pos, dir.getNormalized(), m_BulletSpeeed, m_SpriteName));

		m_CrnCooldown = m_MaxCooldown;
		res.m_Type = SkillResult::ResultType::Bullet;
		return res;
	}

	/// Used after the projectile hit the enemy
	virtual SkillResult OnHit() override;
};

///////////////////////////////////////
// not Normal Attack
///////////////////////////////////////

class NotNormalAtack : public SkillInterface
{
	float m_Dmg;
	float m_BulletSpeeed;
	std::string m_SpriteName;
	SkillInterface& m_PostSkill;

public:
	NotNormalAtack(const float dmg, const float cdn, const float bulletSpeed, const std::string& spriteName, SkillInterface& postCast)
		: m_Dmg(dmg), m_BulletSpeeed(bulletSpeed), m_SpriteName(spriteName), m_PostSkill(postCast)
	{
		m_MaxCooldown = cdn;
	}
	virtual SkillInterface * Clone() override { return new NormalAtack(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_SpriteName); };
	virtual ~NotNormalAtack() override {}

	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override
	{
		SkillResult res;
		res.m_Source = this;
		res.m_Bullets.push_back(new BulletBase(pos, dir.getNormalized(), m_BulletSpeeed, m_SpriteName, 1.f));

		m_CrnCooldown = m_MaxCooldown;
		res.m_Type = SkillResult::ResultType::Bullet;
		return res;
	}

	virtual SkillResult OnHit() override;
	virtual SkillResult OnExplosion(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override;
};



#endif // __DAMAGE_SKILLS_H__
