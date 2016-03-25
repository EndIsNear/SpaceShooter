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
	AIMove GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends);
private:
	std::mt19937 rng;
	const BodyBase& mr_Me;
	const LogicalWeapon& mr_Weapon;
};

#endif // __AI_BASE_H__