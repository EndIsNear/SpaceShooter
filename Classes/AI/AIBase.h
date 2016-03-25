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
	float newVelocity;
};


class AIBase
{
public:
	AIBase(const BodyBase& me, const LogicalWeapon& weapon) : mr_Me(me), mr_Weapon(weapon) { rng.seed(std::random_device()()); }
	virtual AIMove GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt);

protected:
	cocos2d::Vec2 NormalizePtBySize(const cocos2d::Vec2 &pt) const { return cocos2d::Vec2(pt.x / m_AreaSize.width, pt.y / m_AreaSize.height); }
protected:
	cocos2d::Size m_AreaSize;
	std::mt19937 rng;
	const BodyBase& mr_Me;
	const LogicalWeapon& mr_Weapon;
};


class AIPointToPoint : public AIBase
{
public:
	AIPointToPoint(const BodyBase& me, const LogicalWeapon& weapon) :m_NextPtIdx(0) , AIBase(me, weapon) {}
	void AddPoint(cocos2d::Vec2 &rPt) { m_Pattern.push_back(rPt);}
	virtual AIMove GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt) override;

private:
	//contain only norm pt
	unsigned m_NextPtIdx;
	std::vector<cocos2d::Vec2> m_Pattern;
};


class AICoward : public AIBase
{
public:
	AICoward(const BodyBase& me, const LogicalWeapon& weapon, const float minDist, const float maxDist) :
		AIBase(me, weapon)
		, m_MinDist(minDist)
		, m_MaxDist(maxDist)
		{}
	virtual AIMove GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt) override;
private:
	float m_MinDist;
	float m_MaxDist;
};



#endif // __AI_BASE_H__