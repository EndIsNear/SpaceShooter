#include "BattleLogic\Skills\NormalAttack.h"
#include "BattleLogic\LogicalShip.h"

SkillResult NormalAtack::OnHit()
{
	SkillResult res;
	res.m_Effect = SkillEffect(SkillEffect::EffectType::OneTime, [this](LogicalShip* ship)
	{
		ship->TakeDmg(m_Dmg);
	});
	return res;
}
