#include "AI\AIGroupCommander.h"



AITriangle::AITriangle()
{
	m_UnitPositions.resize(3);
	cocos2d::Vec2 dir = cocos2d::Vec2(1., 0);
	float length = 700.;
	m_UnitPositions[0] = dir * length;
	m_UnitPositions[1] = dir.rotateByAngle(cocos2d::Vec2(0., 0.), 3.141592f * 3.141592f / 0.3333f) * length;
	m_UnitPositions[2] = dir.rotateByAngle(cocos2d::Vec2(0., 0.), 3.141592f * 3.141592f / 0.6666f) * length;

	m_FreeIndexes.resize(3);
	m_FreeIndexes[0] = 0;
	m_FreeIndexes[1] = 1;
	m_FreeIndexes[2] = 2;
}

//virual override
void AITriangle::Update(float dt)
{
	for (auto &it : m_UnitPositions)
	{
		it = it.rotateByAngle(cocos2d::Vec2(0., 0.), 3.141592 * 0.1  * dt);
	}
	m_Position = m_Enemy->GetPosition();
}

TestCommanger *TestCommanger::instance = NULL;
