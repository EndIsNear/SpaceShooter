#ifndef  __DAMAGE_SKILLS_H__
#define  __DAMAGE_SKILLS_H__

#include "BattleLogic\Skills\SkillBase.h"

///////////////////////////////////////
// Normal Attack
///////////////////////////////////////

class NormalAttack : public SkillInterface
{
	float m_Dmg;
	float m_BulletSpeeed;
	std::string m_SpriteName;

public:
	NormalAttack(const float dmg, const float cdn, const float bulletSpeed, const std::string& spriteName)
		: m_Dmg(dmg), m_BulletSpeeed(bulletSpeed), m_SpriteName(spriteName)
	{
		m_MaxCooldown = cdn;
	}
	virtual SkillInterface * Clone() override { return new NormalAttack(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_SpriteName); };
	virtual ~NormalAttack() override {}

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
// Spread Attack
///////////////////////////////////////

class NormalSpreadAttack : public SkillInterface
{
	float m_Dmg;
	float m_BulletSpeeed;
	float m_SpreadAngle;
	size_t m_BulletCnt;
	std::string m_SpriteName;

public:
	NormalSpreadAttack(const float dmg, const float cdn, const float bulletSpeed, const float spreadAngle,
		const size_t bulletCnt, const std::string& spriteName)
		: m_Dmg(dmg), m_BulletSpeeed(bulletSpeed), m_SpriteName(spriteName), m_SpreadAngle(spreadAngle), m_BulletCnt(bulletCnt)
	{
		m_MaxCooldown = cdn;
	}
	virtual SkillInterface * Clone() override { return new NormalSpreadAttack(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_SpreadAngle, m_BulletCnt, m_SpriteName); };
	virtual ~NormalSpreadAttack() override {}

	/// Used when there is command to cast a skill
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override
	{
		SkillResult res;
		res.m_Source = this;
		res.m_Bullets.push_back(new BulletBase(pos, dir.getNormalized(), m_BulletSpeeed, m_SpriteName));
		for (size_t i = 1; i < m_BulletCnt; ++i)
		{
			res.m_Bullets.push_back(new BulletBase(pos, dir.getNormalized().rotateByAngle(cocos2d::Vec2::ZERO,
				CC_DEGREES_TO_RADIANS(cocos2d::RandomHelper::random_real(-m_SpreadAngle, m_SpreadAngle))), m_BulletSpeeed, m_SpriteName));
		}

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
// Granade Attack
///////////////////////////////////////

class GranadeAttack : public SkillInterface
{
	float m_Dmg;
	float m_BulletSpeeed;
	float m_ExplosionDelay;
	std::string m_SpriteName;
	SkillInterface& m_PostSkill;

public:
	GranadeAttack(const float dmg, const float cdn, const float bulletSpeed, const float explosionDelay,
		const std::string& spriteName, SkillInterface& postCast)
		: m_Dmg(dmg), m_BulletSpeeed(bulletSpeed), m_SpriteName(spriteName),
			m_PostSkill(postCast), m_ExplosionDelay(explosionDelay)
	{
		m_MaxCooldown = cdn;
	}
	virtual SkillInterface * Clone() override { return new GranadeAttack(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_ExplosionDelay, m_SpriteName, /*TODO: not rly proper*/m_PostSkill); };
	virtual ~GranadeAttack() override {}

	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override
	{
		SkillResult res;
		res.m_Source = this;
		res.m_Bullets.push_back(new GranadeBullet(pos, dir.getNormalized(), m_BulletSpeeed, m_SpriteName, m_ExplosionDelay));

		m_CrnCooldown = m_MaxCooldown;
		res.m_Type = SkillResult::ResultType::Bullet;
		return res;
	}

	virtual SkillResult OnHit() override;
	virtual SkillResult OnExplosion(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override;
};



#endif // __DAMAGE_SKILLS_H__
