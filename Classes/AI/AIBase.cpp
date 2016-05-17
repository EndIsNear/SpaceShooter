#include "AI\AIBase.h"
#include "AI\AIGroupCommander.h"
#include "AI\AIGroupMember.h"

AIBaseInterface * GetAIbyID(const BodyBase& rMe, const LogicalWeapon &mWeapon, unsigned type)
{
	AIBaseStruct ai{ cocos2d::Size(1., 1.), rMe, mWeapon };
	switch (type)
	{
		case 0:
			return new AIGroupMember(ai, TestCommanger::GetInstace(), TestCommanger::GetInstace().GetFreePosition());
		case 1:
			return new JustShoot(ai);
		default:
			return new JustShoot(ai);
	}

}