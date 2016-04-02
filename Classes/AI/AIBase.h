#ifndef __AI_BASE_H__
#define __AI_BASE_H__

#include "cocos2d.h"
#include "Physics\ShipBase.h"
#include "Physics\BulletBase.h"
#include "BattleLogic\LogicalWeapon.h"
#include <random>


struct AIMove
{
	enum FireType : unsigned char
	{
		None = 0,
		NormalAttack =  (1<<0),
		Skill1 = (1 << 1),
		Skill2 = (1 << 2),
		Skill3 = (1 << 3)
	}  fire;
	cocos2d::Vec2 newDir;
	float newVelocity;//normalized (0 - 1)
};

struct AIBaseStruct
{
	cocos2d::Size m_AreaSize;
	std::mt19937 rng;
	const BodyBase& mr_Me;
	const LogicalWeapon& mr_Weapon;
};

template <int VELOCITY>
class AIDefaultMove
{
public:
	AIDefaultMove(const AIBaseStruct& parent)
		:
		m_ParentRef(parent)
	{

	}

	bool GetMove(std::vector<ShipBase*>& enemy,
		std::vector<BulletBase*>& enemyBullets,
		std::vector<ShipBase*>& friends,
		const float dt,
		AIMove &rResult)
	{
		rResult.fire = AIMove::None;
		rResult.newDir = m_ParentRef.mr_Me.GetDirection();
		rResult.newVelocity = VELOCITY;
		return true;
	}

private:
	const AIBaseStruct& m_ParentRef;

};


class  AIStayBehindEnemy
{
public:
		AIStayBehindEnemy(const AIBaseStruct& parent)
			:
			m_ParentRef(parent)
		{
		}

		bool GetMove(std::vector<ShipBase*>& enemy,
			std::vector<BulletBase*>& enemyBullets,
			std::vector<ShipBase*>& friends,
			const float dt,
			AIMove &rResult)
		{
			const BodyBase& me = m_ParentRef.mr_Me;
			const float hisPOV = enemy[0]->GetDirection().getAngle(me.GetPosition() - enemy[0]->GetPosition());
			if (fabs(hisPOV) > 0.5f)
			{
				rResult.newVelocity = 0.01 * me.GetVelocityNormalized();
				return true;
			}
			return false;
		}

private:
	const AIBaseStruct& m_ParentRef;
};


template <int DISTANCE>
class AIStayAway
{
public:
	AIStayAway(const AIBaseStruct& parent)
		:
		m_ParentRef(parent)
	{
	}

	bool GetMove(std::vector<ShipBase*>& enemy,
		std::vector<BulletBase*>& enemyBullets,
		std::vector<ShipBase*>& friends,
		const float dt,
		AIMove &rResult)
	{
		const BodyBase& me = m_ParentRef.mr_Me;
		float dist = me.GetPosition().getDistance(enemy[0]->GetPosition());
		if (dist < DISTANCE || m_Timer > 0.01f)
		{
			const float angle = me.GetDirection().getAngle(me.GetPosition() - enemy[0]->GetPosition());
			rResult.fire = AIMove::None;
			rResult.newVelocity = 1.;
			rResult.newDir = me.GetDirection().rotateByAngle(cocos2d::Vec2(0.f, 0.f), angle * dt / 0.2/*acelerator*/);
			if (dist < DISTANCE)
			{ // reset time
				m_Timer = 0.7f;
			}
			m_Timer -= dt;
			return true;
		}
		return false;
	}
private:
	float m_Timer;
	const AIBaseStruct& m_ParentRef;
};

class AIStrikeOnCd
{
public:
	AIStrikeOnCd(const AIBaseStruct& parent)
		:
		m_ParentRef(parent)
	{

	}

	bool GetMove(std::vector<ShipBase*>& enemy,
		std::vector<BulletBase*>& enemyBullets,
		std::vector<ShipBase*>& friends,
		const float dt,
		AIMove &rResult)
	{
		if (m_ParentRef.mr_Weapon.CanShoot())
		{
			const BodyBase&  me = m_ParentRef.mr_Me;
			const float angle = me.GetDirection().getAngle((enemy[0]->GetPosition() - me.GetPosition()));
			if(fabs(angle) < 0.1f)
			{
				rResult.newDir = me.GetDirection();//(enemy[0]->GetPosition() - me.GetPosition()).getNormalized();
				rResult.fire = AIMove::NormalAttack;
			}
			else
			{
				rResult.newDir = me.GetDirection().rotateByAngle(cocos2d::Vec2(0.f, 0.f), angle * dt / 0.2/*acelerator*/);
				rResult.fire = AIMove::None;
			}
			const float hisPOV = enemy[0]->GetDirection().getAngle(me.GetPosition() - enemy[0]->GetPosition());
			rResult.newVelocity = (hisPOV > 0.5f ? 0.1f : 0.9f )* me.GetVelocityNormalized();
			return true;
		}
		return false;
	}

private:
	const AIBaseStruct& m_ParentRef;

};


class AIBaseInterface
{
public:
	virtual AIMove GetMove(std::vector<ShipBase*>& enemy, 
						std::vector<BulletBase*>& enemyBullets, 
						std::vector<ShipBase*>& friends, 
						const float dt) = 0;
};

template <typename AIPolicy1, typename AIPolicy2, typename AIBaseStructT>
class AIListNode
{
public:
	AIListNode(const AIBaseStructT &r)
		:
		m_policy1(r),
		m_policy2(r)
	{}


		bool GetMove(std::vector<ShipBase*>& enemy,
			std::vector<BulletBase*>& enemyBullets,
			std::vector<ShipBase*>& friends,
			const float dt,
			AIMove &rResult)
	{
		// this method give oportinuty  to build list of different Policy
		if (m_policy1.GetMove(enemy, enemyBullets, friends, dt, rResult))
		{
			m_policy2.GetMove(enemy, enemyBullets, friends, dt, rResult);
			return true;
		}
		else
		{
			return false;
		}
	}

protected:
	AIPolicy1 m_policy1;
	AIPolicy2 m_policy2;
};

template <typename AIPolicy1, typename AIPolicy2, typename AIBaseStructT>
class AITreeNodeBase : public AIBaseStructT , public AIBaseInterface
{
public:

	AITreeNodeBase(const AIBaseStructT &r)
		:
		m_policy1(*this),
		m_policy2(*this),
		AIBaseStructT(r)
		{}

	virtual AIMove GetMove(std::vector<ShipBase*>& enemy,
							std::vector<BulletBase*>& enemyBullets, 
							std::vector<ShipBase*>& friends, 
							const float dt) override
	{
		AIMove result;
		// branching
		if (!m_policy1.GetMove(enemy, enemyBullets, friends, dt, result /*out*/))
		{
			m_policy2.GetMove(enemy, enemyBullets, friends, dt, result /*out*/);
		}
		return result;
	}

	bool GetMove(std::vector<ShipBase*>& enemy,
		std::vector<BulletBase*>& enemyBullets,
		std::vector<ShipBase*>& friends,
		const float dt,
		AIMove &rResult)
	{
		// this method give oportinuty to build tree of different Policy
		return m_policy1.GetMove(enemy, enemyBullets, friends, dt, rResult) 
			|| m_policy2.GetMove(enemy, enemyBullets, friends, dt, rResult);
	}

protected:
	AIPolicy1 m_policy1;
	AIPolicy2 m_policy2;
};


typedef AITreeNodeBase <AIStayAway<600>, AIDefaultMove<100>, AIBaseStruct> StayAwayPolicy;

typedef AIListNode <AIStrikeOnCd, AIStayBehindEnemy, AIBaseStruct> StayBehindAndShootPolicy;
typedef AITreeNodeBase <StayBehindAndShootPolicy, StayAwayPolicy, AIBaseStruct> ShootOrRunPolicy;
typedef AITreeNodeBase <AIStayAway<250>, ShootOrRunPolicy, AIBaseStruct> StayAwayEndShoot;


#endif // __AI_BASE_H__