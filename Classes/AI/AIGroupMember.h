#ifndef AI_GROUP_MEMBER_H
#define AI_GROUP_MEMBER_H

#include "AI\AIBase.h"
#include "AI\AIGroupCommander.h"

class AIGroupMember : public AIBaseInterface
{
public:
	AIGroupMember(const AIBaseStruct& ship,const  GroupCommander& comander, size_t indexInGroup);
	virtual ~AIGroupMember();

	virtual AIMove GetMove(std::vector<ShipBase*>& enemy,
		std::vector<BulletBase*>& enemyBullets,
		std::vector<ShipBase*>& friends,
		const float dt);
private:
	const GroupCommander& m_Commander;
	AIBaseStruct    m_ShipRef;
	size_t			m_IdxInGroup;
};


#endif