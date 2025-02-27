#include "BattleLogic\Skills\DamageSkills.h"
#include "BattleLogic\LogicalShip.h"

SkillResult DamageSkillBase::OnHit()
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

SkillResult GranadeAttack::OnHit()
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::OneTime, EffectFunc([this](LogicalShip* ship, const float dt, float& timeLeft) -> bool
	{
		ship->TakeDmg(m_Dmg);
		return false;
	}));
	return res;
}

SkillResult GranadeAttack::OnExplosion(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir)
{
	const int size = 10;
	const float angleStep = 6.2831f / size;
	SkillResult res;
	res.m_Type = SkillResult::ResultType::Bullet;
	for (int i = 0; i < size; ++i)
	{
		const float angle = angleStep * i;
		res.m_Bullets.push_back(m_PostSkill.OnCast(pos, dir.getNormalized().rotateByAngle(cocos2d::Vec2::ZERO, angle)).m_Bullets[0]);
	}
	m_PostSkill.SetCrnCooldown(0.f);
	res.m_Source = &m_PostSkill;
	return res;

}
