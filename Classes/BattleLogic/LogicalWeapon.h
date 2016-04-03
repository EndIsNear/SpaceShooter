#ifndef  __LOGICAL_WEAPON_H__
#define  __LOGICAL_WEAPON_H__

#include "Utils\FileReader.h"

class LogicalWeapon
{
public:
	LogicalWeapon();
	LogicalWeapon(const float dmg, const float delay, const float bulletSpeed);
	LogicalWeapon(const LogicalWeapon& wpn);
	LogicalWeapon& operator=(const LogicalWeapon& wpn);

	void Update(const float dt);

	float GetDamage() const { return m_Dmg; }
	float GetBulletSpeed() const { return m_BulletSpeed; }
	bool CanShoot() const { return m_ShootCrnDelay < 0.f; }
	void Shoot() { m_ShootCrnDelay = m_ShootMaxDelay; }

	bool Load(const rapidjson::GenericValue<rapidjson::UTF8<> > &entry);
private:
	float m_Dmg;
	float m_BulletSpeed;
	float m_ShootMaxDelay;
	float m_ShootCrnDelay;
};

#endif // __LOGICAL_WEAPON_H__
