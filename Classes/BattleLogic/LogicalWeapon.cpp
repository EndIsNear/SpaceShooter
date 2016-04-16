#include "LogicalWeapon.h"

LogicalWeapon::LogicalWeapon()
{
	m_Skills[0] = new NormalAtack(50.f, 0.5f, 800.f, "bullet.png");
	m_Skills[1] = new NormalAtack(5.f, 0.01f, 800.f, "EnemyBullet.png");
	m_Skills[2] = new NormalAtack(200.f, 1.f, 800.f, "EnemyBullet.png");
	m_Skills[3] = new DoTAttack(500.f, 2.f, 800.f, "bullet2.png", 10.f);
}


LogicalWeapon::LogicalWeapon(const LogicalWeapon& wpn)
{
	for (size_t i = 0; i < skillSize; ++i)
	{
		m_Skills[i] = wpn.m_Skills[i]->Clone();
	}
}

LogicalWeapon& LogicalWeapon::operator=(const LogicalWeapon& wpn)
{
	if (this != &wpn)
	{
		for (size_t i = 0; i < skillSize; ++i)
		{
			delete m_Skills[i];
			m_Skills[i] = wpn.m_Skills[i]->Clone();
		}
	}
	return *this;
}
