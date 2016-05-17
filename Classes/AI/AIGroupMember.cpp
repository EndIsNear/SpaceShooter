#include "AIGroupMember.h"


AIGroupMember::AIGroupMember(const AIBaseStruct& ship, const  GroupCommander& commander, size_t indexInGroup)
	:
	m_ShipRef(ship)
	,m_Commander(commander)
	, m_IdxInGroup(indexInGroup)
{
}


//virtual
AIGroupMember ::~AIGroupMember()
{}


//virtual  override
AIMove AIGroupMember::GetMove(std::vector<ShipBase*>& enemy,
	std::vector<BulletBase*>& enemyBullets,
	std::vector<ShipBase*>& friends,
	const float dt)
{
	AIMove result;
	result.fire = UsedSkill::None;

	//TODO: remove const cast
	//if (const_cast<LogicalWeapon&>(m_ShipRef.mr_Weapon).GetCooldownAt(0))
	//{
	//	const BodyBase&  me = m_ShipRef.mr_Me;
	//	const cocos2d::Vec2 enemyDir = (enemy[0]->GetPosition() - me.GetPosition()).getNormalized();
	//	const float angle = me.GetDirection().getAngle(enemyDir);
	//	if (fabs(angle) < 0.1f)
	//	{
	//		result.newDir = enemyDir;
	//		result.fire = UsedSkill::NormalAttack;
	//	}
	//	else
	//		result.newDir = me.GetDirection().rotateByAngle(cocos2d::Vec2(0.f, 0.f), angle * dt / 0.2/*acelerator*/);

	//	const float hisPOV = enemy[0]->GetDirection().getAngle(enemyDir);
	//	result.newVelocity = std::max(dt / (hisPOV > 0.5f ? 0.1f : 0.9f) * 0.5f* me.GetVelocityNormalized(), 0.5f);
	//}
	//else
	//{
		cocos2d::Vec2 desirePos;
		m_Commander.GetPosition(m_IdxInGroup, desirePos);
		result.newDir = desirePos - m_ShipRef.mr_Me.GetPosition();
		result.newDir.normalize();
		result.newVelocity = 1.f;
	//}
	return result;
}


