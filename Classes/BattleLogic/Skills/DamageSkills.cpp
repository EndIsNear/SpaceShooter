#include "BattleLogic\Skills\DamageSkills.h"
#include "BattleLogic\LogicalShip.h"

SkillResult NormalAtack::OnHit()
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::OneTime, EffectFunc([this](LogicalShip* ship, const float dt, float& timeLeft) -> bool
	{
		ship->TakeDmg(m_Dmg);
		return false;
	}));
	return res;
}

SkillResult DoTAttack::OnHit()
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::DoT, EffectFunc([this](LogicalShip* ship, const float dt, float& timeEffect)
	{
		if (timeEffect > 0.f)
		{
			ship->TakeDmg(m_Dmg * dt);
			timeEffect -= dt;
			return true;
		}
		return false;
	}, m_EffectTime));
	return res;
}

SkillResult NotNormalAtack::OnHit()
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::OneTime, EffectFunc([this](LogicalShip* ship, const float dt, float& timeLeft) -> bool
	{
		ship->TakeDmg(m_Dmg);
		return false;
	}));
	return res;
}

SkillResult NotNormalAtack::OnExplosion(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir)
{
	SkillResult res = m_PostSkill.OnCast(pos, dir.getNormalized() * -1.f);
	assert(res.m_Bullet != nullptr);
	return res;

}
