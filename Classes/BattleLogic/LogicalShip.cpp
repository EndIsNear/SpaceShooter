#include "LogicalShip.h"


LogicalShip::LogicalShip(float hp, float shield, float shieldPerSec, float dmg) :
	m_MaxHP(hp),
	m_CrnHP(hp),
	m_HPPerSec(0),
	m_MaxShield(shield),
	m_CrnShield(shield),
	m_ShieldPerSec(shieldPerSec)
{

}

void LogicalShip::update(const float dt)
{
	m_CrnShield += m_ShieldPerSec * dt;
	if (m_CrnShield > m_MaxShield)
		m_CrnShield = m_MaxShield;

	m_CrnHP += m_HPPerSec * dt;
	if (m_CrnHP > m_MaxHP)
		m_CrnHP = m_MaxHP;
}

void LogicalShip::onHit(LogicalWeapon * atacker)
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
