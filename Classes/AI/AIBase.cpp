#include "AIBase.h"


AIMove AIBase::GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt)
{
	std::uniform_real_distribution<> v(0, 5);
	AIMove res;
	res.newVelocity = v(rng);
	res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
	res.fire = AIMove::FireType( v(rng) > 4.8f);
	return res;
}


//virtual override
AIMove AIPointToPoint::GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt)
{
	assert(m_Pattern.size());
	std::uniform_real_distribution<> v(0, 1);
	AIMove res;
	if (mr_Weapon.CanShoot())
	{
		// shoot
		res.newVelocity = 0.;
		res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
		res.fire = AIMove::NormalAttack;
	}
	else
	{
		res.newVelocity = 5.;
		res.newDir = (m_Pattern[m_NextPtIdx] - mr_Me.GetPosition()).getNormalized();
		res.fire = AIMove::None;
	}
	if (m_Pattern[m_NextPtIdx].getDistance(mr_Me.GetPosition()) < 20.)
	{
		m_NextPtIdx++;
		m_NextPtIdx %= m_Pattern.size();
	}
	return res;
}


//virtual  override
AIMove AICoward::GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt)
{
	float dist = mr_Me.GetPosition().getDistance(enemy[0]->GetPosition());
	AIMove res;
	res.newVelocity = 5.;
	if (dist < m_MinDist)	
	{
		res.newDir = (mr_Me.GetPosition() - enemy[0]->GetPosition()).getNormalized();
		res.fire = AIMove::None;
	} 
	else if (dist > m_MaxDist)
	{
		res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();

	}
	else
	{
		res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
		res.fire = AIMove::NormalAttack;
		res.newVelocity = 0.;
	}

	return res;
}
