#ifndef  __DAMAGE_SKILLS_H__
#define  __DAMAGE_SKILLS_H__

#include "BattleLogic\Skills\SkillBase.h"


///////////////////////////////////////
// Damage Skill Base
// It implements simple OnHit method
///////////////////////////////////////

class DamageSkillBase : public SkillInterface
{
protected:
	float m_Dmg;
	float m_BulletSpeeed;
	std::string m_SpriteName;
public:
	DamageSkillBase(const float dmg, const float cdn, const float bulletSpeed, const std::string& spriteName)
		: m_Dmg(dmg), m_BulletSpeeed(bulletSpeed), m_SpriteName(spriteName)
	{
		m_MaxCooldown = cdn;
	}
	virtual ~DamageSkillBase() override {}

	/// Used when there is command to cast a skill
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) = 0;

	/// Used when there is command to cast a skill
	/// Used after the projectile hit the enemy
	virtual SkillResult OnHit() override;
};

///////////////////////////////////////
// Normal Attack
///////////////////////////////////////

class NormalAttackSkill : public DamageSkillBase
{
public:
	NormalAttackSkill(const float dmg, const float cdn, const float bulletSpeed, const std::string& spriteName)
		: DamageSkillBase(dmg, cdn, bulletSpeed, spriteName)
	{}
	virtual SkillInterface * Clone() override { return new NormalAttackSkill(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_SpriteName); };
	virtual ~NormalAttackSkill() override {}

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
};

///////////////////////////////////////
// Spread Attack
///////////////////////////////////////

class SpreadAttackSkill : public DamageSkillBase
{
	float m_SpreadAngle;
	size_t m_BulletCnt;
public:
	SpreadAttackSkill(const float dmg, const float cdn, const float bulletSpeed, const float spreadAngle,
		const size_t bulletCnt, const std::string& spriteName)
		: DamageSkillBase(dmg, cdn, bulletSpeed, spriteName), m_SpreadAngle(spreadAngle), m_BulletCnt(bulletCnt)
	{}
	virtual SkillInterface * Clone() override { return new SpreadAttackSkill(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_SpreadAngle, m_BulletCnt, m_SpriteName); };
	virtual ~SpreadAttackSkill() override {}

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
};

///////////////////////////////////////
// Dual Barrel Attack
///////////////////////////////////////

class DualBarrelAttack : public DamageSkillBase
{
	float m_OffsetFromCenter;
public:
	DualBarrelAttack(const float dmg, const float cdn, const float bulletSpeed, const float offsetFromCenter, const std::string& spriteName)
		: DamageSkillBase(dmg, cdn, bulletSpeed, spriteName), m_OffsetFromCenter(offsetFromCenter)
	{}
	virtual SkillInterface * Clone() override { return new DualBarrelAttack(m_Dmg, m_MaxCooldown, m_BulletSpeeed, m_OffsetFromCenter, m_SpriteName); };
	virtual ~DualBarrelAttack() override {}

	/// Used when there is command to cast a skill
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override
	{
		SkillResult res;
		res.m_Source = this;
		const cocos2d::Vec2 nDir = dir.getNormalized();
		cocos2d::Vec2 offset = nDir.getPerp();
		offset.scale(m_OffsetFromCenter);
		res.m_Bullets.push_back(new BulletBase(pos + offset, nDir, m_BulletSpeeed, m_SpriteName));
		res.m_Bullets.push_back(new BulletBase(pos - offset, nDir, m_BulletSpeeed, m_SpriteName));
		m_CrnCooldown = m_MaxCooldown;
		res.m_Type = SkillResult::ResultType::Bullet;
		return res;
	}
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
