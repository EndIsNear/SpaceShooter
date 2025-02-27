#ifndef  __SKILL_BASE_H__
#define  __SKILL_BASE_H__

#include "Physics\BulletBase.h"

class SkillInterface;
class LogicalShip;

//typedef std::function<bool(LogicalShip*)> EffectFunc;
struct EffectFunc
{
	//return false when the effect ends
	//first float is delta time
	//second crn effect time
	typedef std::function<bool(LogicalShip*, const float, float&)> Func;

	EffectFunc() {}
	EffectFunc(Func f, const float t = 0.f) : m_Func(f), m_EffectTime(t) {}

	bool operator()(LogicalShip* ship, const float dt)
	{
		return m_Func(ship, dt, m_EffectTime);
	}
	//used to DoT (and other effect for given time)
	float m_EffectTime;
	Func m_Func;
};

struct SkillEffect
{
	enum EffectType
	{
		OneTime,
		DoT
	} m_Type;
	EffectFunc m_Func;

	SkillEffect() {}
	SkillEffect(EffectType type, EffectFunc func) : m_Type(type), m_Func(func) {}
};

class SkillResult
{
public:
	enum ResultType
	{
		None,
		Bullet,
		Effect
	} m_Type;
	SkillResult() : m_Source(nullptr), m_Type(None) {}
	std::vector<BulletBase*> m_Bullets;
	SkillEffect m_Effect;

	//always exist ptr to source
	SkillInterface * m_Source;
};

///IMPORTANT: Normal atack should implement this interface too!
class SkillInterface
{
public:
	SkillInterface() { m_CrnCooldown = 0.f; }
	virtual SkillInterface * Clone() = 0;
	virtual ~SkillInterface() {};
	
	/// Used when there is command to cast a skill
	/// If returns effect on cast it have to be applied on the same ship
	virtual SkillResult OnCast(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) = 0;
	/// Used after the projectile hit the enemy
	virtual SkillResult OnHit() = 0;

	/// Used for something
	virtual SkillResult OnExplosion(const cocos2d::Vec2 pos, const cocos2d::Vec2 dir) { return SkillResult(); }

	/// Returns true if can shoot
	virtual bool CanCast()
	{
		return m_CrnCooldown <= 0.f;
	}

	/// Updates the state of the skill, mostly reduce cooldown
	virtual void Update(const float dt)
	{
		m_CrnCooldown -= dt;
	}

	virtual void SetMaxCooldown(const float cdn)
	{
		m_MaxCooldown = cdn;
		if (m_CrnCooldown > cdn) m_CrnCooldown = cdn;
	}

	virtual void SetCrnCooldown(const float cdn)
	{
		m_CrnCooldown = cdn;
	}

	virtual float GetMaxCooldown()
	{
		return m_MaxCooldown;
	}

	virtual float GetCrnCooldown()
	{
		return m_CrnCooldown;
	}

	virtual float GetCooldownInPerc()
	{
		return m_CrnCooldown > FLT_EPSILON ? m_CrnCooldown / m_MaxCooldown : 0.f;
	}

protected:
	float m_CrnCooldown;
	float m_MaxCooldown;
};

#endif // __SKILL_BASE_H__
