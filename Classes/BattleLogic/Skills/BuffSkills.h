#ifndef  __BUFF_SKILLS_H__
#define  __BUFF_SKILLS_H__

#include "BattleLogic\Skills\SkillBase.h"

///////////////////////////////////////
// Increse Attack Speed Skill
///////////////////////////////////////

class IncAttackSpeedSkill : public SkillInterface
{
	float m_EffectTime;
	float m_EffectRatio;
	size_t m_EffectedSkillIdx;
	bool m_EffectApplied;

public:
	IncAttackSpeedSkill(const float cdn, const float effectRatio, const float effTime, size_t effIdx = 0)
		: m_EffectTime(effTime), m_EffectApplied(false), m_EffectRatio(effectRatio), m_EffectedSkillIdx(effIdx)
	{
		m_MaxCooldown = cdn;
	}
	virtual SkillInterface * Clone() override { return new IncAttackSpeedSkill(m_MaxCooldown, m_EffectRatio,  m_EffectTime, m_EffectedSkillIdx); };
	virtual ~IncAttackSpeedSkill() override {}

	/// Used when there is command to cast a skill
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) override;

	/// Used after the projectile hit the enemy
	virtual SkillResult OnHit() override { return SkillResult(); }
};

#endif // __DOT_ATACK_H__
