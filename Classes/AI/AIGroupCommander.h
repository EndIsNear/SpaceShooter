#ifndef __AI_GROUP_COMMANDER_H__
#define __AI_GROUP_COMMANDER_H__

#include "cocos2d.h"
#include "AI\AIBase.h"


class AIGroumMember : public AIBaseInterface
{
	AIGroumMember();
	~AIGroumMember();

	virtual AIMove GetMove(std::vector<ShipBase*>& enemy,
		std::vector<BulletBase*>& enemyBullets,
		std::vector<ShipBase*>& friends,
		const float dt) = 0;
	public:
};

class GroupCommander
{
public:
	struct Token
	{
		GroupCommander& m_Commander;
		size_t			Idx;
	};

	bool GetToken(GroupCommander::Token& token) 
	{
		if (!m_FreeIndexes.empty())
		{
			token.m_Commander = *this;
			token.Idx = m_FreeIndexes.back();
			m_FreeIndexes.pop_back();
			return true;
		}
		return false;
	}
	void ReleaseToken(GroupCommander::Token& token) 
	{
		assert(token.Idx < m_UnitPositions.size());
		m_FreeIndexes.push_back(token.Idx);
		assert(m_FreeIndexes < m_UnitPositions.size());
	}

	virtual void Update(float dt) = 0;
protected:
	GroupCommander() : m_Position(500.f, 500.f) {}

	std::vector<cocos2d::Vec2> m_UnitPositions;
	std::vector<size_t> m_FreeIndexes;
	cocos2d::Vec2 m_Position;
};

class AITriangle : public GroupCommander
{
public:
	AITriangle()
	{
		m_UnitPositions.resize(3);
		cocos2d::Vec2 dir = cocos2d::Vec2(200, 0);
		m_UnitPositions[0] = dir;
		m_UnitPositions[1] = dir.rotateByAngle(cocos2d::Vec2(0., 0.), 3.141592 / 0.3333f);
		m_UnitPositions[2] = dir.rotateByAngle(cocos2d::Vec2(0., 0.), 3.141592 / 0.6666f);
		
		m_FreeIndexes.resize(3);
		m_FreeIndexes[0] = 0;
		m_FreeIndexes[1] = 1;
		m_FreeIndexes[2] = 2;
	}



	virtual void Update(float dt) = 0;
protected:
};


#endif //__AI_GROUP_COMMANDER_H__

