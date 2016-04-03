#include "LogicalShip.h"

LogicalShip::LogicalShip() :
	m_MaxHP(0.f),
	m_CrnHP(0.f),
	m_HPPerSec(0.f),
	m_MaxShield(0.f),
	m_CrnShield(0.f),
	m_ShieldPerSec(0.f),
	m_Weapon(NULL)
{

}


LogicalShip::LogicalShip(float hp, float shield, float shieldPerSec, float healthPerSec, LogicalWeapon * wep) :
	m_MaxHP(hp),
	m_CrnHP(hp),
	m_HPPerSec(healthPerSec),
	m_MaxShield(shield),
	m_CrnShield(shield),
	m_ShieldPerSec(shieldPerSec),
	m_Weapon(wep)
{

}

LogicalShip::LogicalShip(const LogicalShip& ls) :
	m_MaxHP(ls.m_MaxHP),
	m_CrnHP(ls.m_CrnHP),
	m_HPPerSec(ls.m_HPPerSec),
	m_MaxShield(ls.m_MaxShield),
	m_CrnShield(ls.m_CrnShield),
	m_ShieldPerSec(ls.m_ShieldPerSec)
{
	if (ls.m_Weapon) m_Weapon = new LogicalWeapon(*ls.m_Weapon);
}

LogicalShip& LogicalShip::operator=(const LogicalShip& ls)
{
	if (this != &ls)
	{
		m_MaxHP = ls.m_MaxHP;
		m_CrnHP = ls.m_CrnHP;
		m_HPPerSec = ls.m_HPPerSec;
		m_MaxShield = ls.m_MaxShield;
		m_CrnShield = ls.m_CrnShield;
		m_ShieldPerSec = ls.m_ShieldPerSec;
		if (m_Weapon) delete m_Weapon;
		m_Weapon = new LogicalWeapon(*ls.m_Weapon);
	}
	return *this;
}

void LogicalShip::SetLogicalWeapon(LogicalWeapon * newWeapon)
{
	m_Weapon = newWeapon;
}

void LogicalShip::Update(const float dt)
{
	m_CrnShield += m_ShieldPerSec * dt;
	if (m_CrnShield > m_MaxShield)
		m_CrnShield = m_MaxShield;

	m_CrnHP += m_HPPerSec * dt;
	if (m_CrnHP > m_MaxHP)
		m_CrnHP = m_MaxHP;

	m_Weapon->Update(dt);
}

void LogicalShip::OnHit(LogicalWeapon * atacker)
{
	float dmg = atacker->GetDamage();
	if (dmg > m_CrnShield)
	{
		dmg -= m_CrnShield;
		m_CrnShield = 0;
		m_CrnHP -= dmg;
	}
	else
	{
		m_CrnShield -= dmg;
	}
}

bool LogicalShip::Load(const rapidjson::GenericValue<rapidjson::UTF8<> > &entry)
{
	m_MaxHP = entry["MaxHP"].GetDouble();
	m_CrnHP = m_MaxHP;
	m_HPPerSec = entry["HPPerSec"].GetDouble();
	m_MaxShield = entry["MaxShield"].GetDouble();
	m_CrnShield = m_MaxShield;
	m_ShieldPerSec = entry["ShieldPerSec"].GetDouble();
	return true;
}

