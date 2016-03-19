#ifndef  __LOGICAL_SHIP_H__
#define  __LOGICAL_SHIP_H__
#include "LogicalWeapon.h"

class LogicalShip
{
public:
	LogicalShip(float hp, float shield, float shieldPerSec, float dmg);

	void update(const float dt);
	void onHit(LogicalWeapon * atacker);

	bool isAlive() { return m_CrnHP > 0.f; }
	float getLifeInPer() { return m_CrnHP / m_MaxHP; };
	float getShieldInPer() { return m_CrnShield / m_MaxShield; };
private:
	float m_MaxHP;
	float m_CrnHP;
	float m_HPPerSec;
	float m_MaxShield;
	float m_CrnShield;
	float m_ShieldPerSec;

	LogicalWeapon m_Weapon;
};

#endif // __LOGICAL_SHIP_H__
