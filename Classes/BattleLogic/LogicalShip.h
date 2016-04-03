#ifndef  __LOGICAL_SHIP_H__
#define  __LOGICAL_SHIP_H__
#include "LogicalWeapon.h"

class LogicalShip
{
public:
	LogicalShip(float hp, float shield, float shieldPerSec, float healthPerSec, LogicalWeapon * wep = nullptr);
	LogicalShip(const LogicalShip& ls);
	LogicalShip& operator=(const LogicalShip& ls);

	void Update(const float dt);
	void OnHit(LogicalWeapon * atacker);

	bool IsAlive() const { return m_CrnHP > 0.f; }
	float GetLifeInPer() const { return (m_CrnHP / m_MaxHP) * 100.f; }
	float GetShieldInPer() const { return (m_CrnShield / m_MaxShield) * 100.f; }

	const LogicalWeapon * GetWeapon() const { return m_Weapon; }
	LogicalWeapon * GetWeapon() { return m_Weapon; }
private:
	float m_MaxHP;
	float m_CrnHP;
	float m_HPPerSec;
	float m_MaxShield;
	float m_CrnShield;
	float m_ShieldPerSec;

	LogicalWeapon * m_Weapon;
};

#endif // __LOGICAL_SHIP_H__
