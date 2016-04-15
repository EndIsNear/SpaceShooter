#include "LogicalWeapon.h"

LogicalWeapon::LogicalWeapon()
{
	for (size_t i = 0; i < skillSize; ++i)
	{
		m_Skills[i] = new NormalAtack;
	}
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
