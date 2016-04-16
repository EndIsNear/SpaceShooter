#include "BattleLogic\Skills\DoTAttack.h"
#include "BattleLogic\LogicalShip.h"

SkillResult DoTAttack::OnHit()
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::DoT, EffectFunc([this](LogicalShip* ship, const float dt, float timeEffect)
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
