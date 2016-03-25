#include "AIBase.h"


std::mt19937 rng;
AIMove AIBase::NextMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends)
{
	std::uniform_real_distribution<> v(0, 5);
	AIMove res;
	res.newVelocity = v(rng);
	res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
	res.fire = AIMove::FireType( v(rng) > 4.8f);
	return res;
}