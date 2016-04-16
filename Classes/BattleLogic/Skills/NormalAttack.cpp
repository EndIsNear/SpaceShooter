#include "BattleLogic\Skills\NormalAttack.h"
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
