#include "BattleLogic\Skills\BuffSkills.h"
#include "BattleLogic\LogicalShip.h"


SkillResult IncAttackSpeedSkill::OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir)
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::OneTime, EffectFunc([this](LogicalShip* ship, const float dt, float& timeLeft) -> bool
	{
		if (!m_EffectApplied)
		{
			const float cdn = ship->GetWeapon()->GetSkillAt(m_EffectedSkillIdx)->GetMaxCooldown();
			ship->GetWeapon()->GetSkillAt(m_EffectedSkillIdx)->SetMaxCooldown(cdn / m_EffectRatio);
			m_EffectApplied = true;
		}

		timeLeft -= dt;

		if (timeLeft < 0.f)
		{
			const float cdn = ship->GetWeapon()->GetSkillAt(m_EffectedSkillIdx)->GetMaxCooldown();
			ship->GetWeapon()->GetSkillAt(m_EffectedSkillIdx)->SetMaxCooldown(cdn * m_EffectRatio);
			m_EffectApplied = false;
			return false;
		}

		return true;
	}, m_EffectTime));
	m_CrnCooldown = m_MaxCooldown;
	res.m_Type = SkillResult::ResultType::Effect;
	return res;
}
