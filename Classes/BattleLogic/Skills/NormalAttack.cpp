#include "BattleLogic\Skills\NormalAttack.h"
#include "BattleLogic\LogicalShip.h"

SkillResult NormalAtack::OnHit()
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::OneTime, [](LogicalShip* ship)
	{
		ship->TakeDmg(100.f);
	});
	return res;
}
