#ifndef __AI_BASE_H__
#define __AI_BASE_H__

#include "cocos2d.h"
#include "Physics\ShipBase.h"
#include "Physics\BulletBase.h"
#include "BattleLogic\LogicalWeapon.h"
#include <random>
#include <algorithm>


struct AIMove
{
	UsedSkill fire;
	cocos2d::Vec2 newDir;
	float newVelocity;//normalized (0 - 1)
};


class Boreder
{
public:
	static float GetDistanceByPt(const cocos2d::Size size,const cocos2d::Vec2 pt)
	{
		float h = std::min(pt.y / size.height, size.height - pt.y/ size.height);
		float v = std::min(pt.x / size.width, size.width - pt.x / size.height);
		return  std::min(h, v);
	}
};

struct AIBaseStruct
{
	cocos2d::Size m_AreaSize;
	const BodyBase& mr_Me;
	const LogicalWeapon& mr_Weapon;
};



class AIBaseInterface
{
public:
	virtual AIMove GetMove(std::vector<ShipBase*>& enemy,
		std::vector<BulletBase*>& enemyBullets,
		std::vector<ShipBase*>& friends,
		const float dt) = 0;
};

AIBaseInterface * GetAIbyID(const BodyBase& rMe, const LogicalWeapon &mWeapon, unsigned type);

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
		rResult.fire = UsedSkill::None;
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
				rResult.newVelocity = std::max((dt/ 0.5f) * 0.5f * me.GetVelocityNormalized(), 0.5f);
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
			rResult.fire = UsedSkill::None;
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
		//TODO: remove const cast
		if (const_cast<LogicalWeapon&>(m_ParentRef.mr_Weapon).GetCooldownAt(0))
		{
			const BodyBase&  me = m_ParentRef.mr_Me;
			const cocos2d::Vec2 enemyDir = (enemy[0]->GetPosition() - me.GetPosition()).getNormalized();
			const float angle = me.GetDirection().getAngle(enemyDir);
			if(fabs(angle) < 0.1f)
			{
				rResult.newDir = enemyDir;
				rResult.fire = UsedSkill::NormalAttack;
			}
			else
			{
				rResult.newDir = me.GetDirection().rotateByAngle(cocos2d::Vec2(0.f, 0.f), angle * dt / 0.2/*acelerator*/);
				rResult.fire = UsedSkill::None;
			}
			const float hisPOV = enemy[0]->GetDirection().getAngle(enemyDir);
			rResult.newVelocity = std::max(dt/(hisPOV > 0.5f ? 0.1f : 0.9f ) * 0.5f* me.GetVelocityNormalized(), 0.5f);
			return true;
		}
		return false;
	}

private:
	const AIBaseStruct& m_ParentRef;

};


class AIStayAwayFromWall
{
public:
	AIStayAwayFromWall(const AIBaseStruct& parent)
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
		const float dist = Boreder::GetDistanceByPt(m_ParentRef.m_AreaSize, m_ParentRef.mr_Me.GetPosition());
		if (dist <  0.1)
		{

			return true;
		}
		return false;
	}

private:
	const AIBaseStruct& m_ParentRef;

};


template <typename AIPolicy1, typename AIPolicy2, typename AIBaseStructT = AIBaseStruct>
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

template <typename AIPolicy1, typename AIPolicy2, typename AIBaseStructT = AIBaseStruct>
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



typedef AITreeNodeBase <AIStayAway<600>, AIDefaultMove<100>> StayAwayPolicy;

typedef AIListNode <AIStrikeOnCd, AIStayBehindEnemy> StayBehindAndShootPolicy;
typedef AITreeNodeBase <StayBehindAndShootPolicy, StayAwayPolicy> ShootOrRunPolicy;
typedef AITreeNodeBase <AIStayAway<250>, ShootOrRunPolicy> StayAwayAndShoot;
typedef AITreeNodeBase <ShootOrRunPolicy, AIStayAway<250>> ShootAndStayAway;
typedef AIListNode <AIStayAwayFromWall, StayAwayAndShoot> WallAvoid;
typedef AITreeNodeBase <WallAvoid, ShootAndStayAway> MegaOpAI;
typedef AITreeNodeBase < AIStrikeOnCd, AIDefaultMove<1>> JustShoot;

#endif // __AI_BASE_H__