#include "LogicalWeapon.h"

LogicalWeapon::LogicalWeapon(const float dmg, const float delay, const float bulletSpeed) :
	m_Dmg(dmg),
	m_ShootMaxDelay(delay),
	m_ShootCrnDelay(0.f),
	m_BulletSpeed(bulletSpeed)
{
	
}

void LogicalWeapon::Update(const float dt)
{
	m_ShootCrnDelay -= dt;
}
