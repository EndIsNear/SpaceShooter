#include "LogicalWeapon.h"

LogicalWeapon::LogicalWeapon(std::unique_ptr<SkillInterface>& skill0, std::unique_ptr<SkillInterface>& skill1,
	std::unique_ptr<SkillInterface>& skill2, std::unique_ptr<SkillInterface>& skill3) :
	m_Skills{ std::move(skill0),
		std::move(skill1),
		std::move(skill2),
		std::move(skill3)
}
{}


LogicalWeapon::LogicalWeapon(const LogicalWeapon& wpn)
{
	for (size_t i = 0; i < skillSize; ++i)
	{
		m_Skills[i].reset(wpn.m_Skills[i]->Clone());
	}
}

LogicalWeapon& LogicalWeapon::operator=(const LogicalWeapon& wpn)
{
	if (this != &wpn)
	{
		for (size_t i = 0; i < skillSize; ++i)
		{
			m_Skills[i].reset(wpn.m_Skills[i]->Clone());
		}
	}
	return *this;
}
