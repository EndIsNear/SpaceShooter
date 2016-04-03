#include "LogicalWeapon.h"

LogicalWeapon::LogicalWeapon() :
	m_Dmg(0.f),
	m_ShootMaxDelay(0.f),
	m_ShootCrnDelay(0.f),
	m_BulletSpeed(0.f)
{

}


LogicalWeapon::LogicalWeapon(const float dmg, const float delay, const float bulletSpeed) :
	m_Dmg(dmg),
	m_ShootMaxDelay(delay),
	m_ShootCrnDelay(0.f),
	m_BulletSpeed(bulletSpeed)
{
	
}

LogicalWeapon::LogicalWeapon(const LogicalWeapon& wpn) :
	m_Dmg(wpn.m_Dmg),
	m_ShootMaxDelay(wpn.m_ShootMaxDelay),
	m_ShootCrnDelay(0.f),
	m_BulletSpeed(wpn.m_BulletSpeed)
{

}

LogicalWeapon& LogicalWeapon::operator=(const LogicalWeapon& wpn)
{
	if (this != &wpn)
	{
		m_Dmg = wpn.m_Dmg;
		m_ShootMaxDelay = wpn.m_ShootMaxDelay;
		m_ShootCrnDelay = 0.f;
		m_BulletSpeed = wpn.m_BulletSpeed;
	}
	return *this;
}


void LogicalWeapon::Update(const float dt)
{
	m_ShootCrnDelay -= dt;
}

bool LogicalWeapon::Load(const rapidjson::GenericValue<rapidjson::UTF8<> > &entry)
{
	m_Dmg = entry["Dmg"].GetDouble();
	m_BulletSpeed = entry["BulletSpeed"].GetDouble();
	m_ShootMaxDelay = entry["ShootMaxDelay"].GetDouble();
	m_ShootCrnDelay = 0.;
	return true;
}
