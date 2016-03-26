#include "AIBase.h"


AIMove AIBase::GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt)
{
	std::uniform_real_distribution<> v(0, 1);
	AIMove res;
	res.newVelocity = v(rng);
	res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
	res.fire = AIMove::FireType( v(rng) > 0.9f);
	return res;
}


//virtual override
AIMove AIPointToPoint::GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt)
{
	assert(m_Pattern.size());
	std::uniform_real_distribution<> v(0, 1);
	AIMove res;
	if (mr_Weapon.CanShoot() )
	{
		// shoot
<<<<<<< HEAD
		res.newVelocity = 1.5;
		const float angle = mr_Me.GetDirection().getAngle((enemy[0]->GetPosition() - mr_Me.GetPosition()));
		//std::string debugString;
		//debugString  = angle;
		if(fabs(angle) < 0.1f)
		{
			res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
			res.fire = AIMove::NormalAttack;
		}
		else
		{
			//OutputDebugStringA("AtackPrepare");
			res.newDir = Animation(angle, dt).getNormalized();
			res.fire = AIMove::None;
		}
=======
		res.newVelocity = 0.f;
		res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
		res.fire = AIMove::NormalAttack;
>>>>>>> master
	}
	else 
	{
<<<<<<< HEAD
		res.newVelocity = 5.f;
=======
		res.newVelocity = 0.5f;
>>>>>>> master
		res.newDir = (m_Pattern[m_NextPtIdx] - mr_Me.GetPosition()).getNormalized();
		res.fire = AIMove::None;
	}
	if (m_Pattern[m_NextPtIdx].getDistance(mr_Me.GetPosition()) < 20.f)
	{
		m_NextPtIdx++;
		m_NextPtIdx %= m_Pattern.size();
	}
	return res;
}

const cocos2d::Vec2 AIBase::Animation(float angle, double dt) const
{
	return mr_Me.GetDirection().rotateByAngle(cocos2d::Vec2(0.f, 0.f), angle * (dt / 0.15f /*acselerator*/));
}



//virtual  override
AIMove AICoward::GetMove(std::vector<ShipBase*>& enemy, std::vector<BulletBase*>& enemyBullets, std::vector<ShipBase*>& friends, const float dt)
{
	float dist = mr_Me.GetPosition().getDistance(enemy[0]->GetPosition());
	AIMove res;
<<<<<<< HEAD
	res.newDir = cocos2d::Vec2(1., 0.);
	float angle = 0.;
	for (int i = 0; i < enemyBullets.size(); i++)
=======
	res.newVelocity = 0.5f;
	if (dist < m_MinDist)	
>>>>>>> master
	{
		cocos2d::Vec2 dir = mr_Me.GetPosition() - enemyBullets[i]->GetPosition();
		if (fabs(dir.getAngle(enemyBullets[i]->GetDirection())) < 0.351f)
		{
			res.newDir = enemyBullets[i]->GetDirection().getPerp().getNormalized();
			res.newVelocity = 10.f;
			return res;
		}
	}
	if (mr_Weapon.CanShoot() && m_Timer< 0.0001f)
	{
		// shoot
		angle = mr_Me.GetDirection().getAngle((enemy[0]->GetPosition() - mr_Me.GetPosition()));
		res.fire = fabs(angle) < 0.1f ? AIMove::NormalAttack : AIMove::None;
	}
	else if (dist < m_MinDist )	
	{
<<<<<<< HEAD
		// too close
		m_Timer = m_Timer < 0.0001f ? m_Timer = 0.2 /*start new move*/: m_Timer/*keep runing*/;
		angle = mr_Me.GetDirection().getAngle((mr_Me.GetPosition() - enemy[0]->GetPosition()));
		res.fire = AIMove::None;
	} 
	else if (dist > m_MaxDist)
	{	
		// too far
		angle = mr_Me.GetDirection().getAngle(enemy[0]->GetPosition() - mr_Me.GetPosition());
=======
		res.newDir = (enemy[0]->GetPosition() - mr_Me.GetPosition()).getNormalized();
		res.fire = AIMove::NormalAttack;
		res.newVelocity = 0.5f;
>>>>>>> master
	}
	res.newVelocity = 5.f;
	res.newDir = Animation(angle, dt).getNormalized();
	m_Timer -= dt;
	return res;
}
