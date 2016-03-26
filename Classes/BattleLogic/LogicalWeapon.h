#ifndef  __LOGICAL_WEAPON_H__
#define  __LOGICAL_WEAPON_H__

class LogicalWeapon
{
public:
	LogicalWeapon(const float dmg, const float delay, const float bulletSpeed);

	void Update(const float dt);

	float GetDamage() const { return m_Dmg; }
	float GetBulletSpeed() const { return m_BulletSpeed; }
	bool CanShoot() const { return m_ShootCrnDelay < 0.f; }
	void Shoot() { m_ShootCrnDelay = m_ShootMaxDelay; }
private:
	float m_Dmg;
	float m_BulletSpeed;
	float m_ShootMaxDelay;
	float m_ShootCrnDelay;
};

#endif // __LOGICAL_WEAPON_H__
