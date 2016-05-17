#ifndef __AI_GROUP_COMMANDER_H__
#define __AI_GROUP_COMMANDER_H__

#include "cocos2d.h"
#include "AI\AIBase.h"
#include "Physics\ShipBase.h"
#include "Physics\BulletBase.h"
#include "BattleLogic\LogicalWeapon.h"
#include <random>
#include <algorithm>


class GroupCommander
{
public:
	struct Token
	{
		Token(GroupCommander& commander, size_t idx) : m_Commander(commander), m_Idx(idx) {}
		GroupCommander& m_Commander;
		size_t			m_Idx;
	};

	size_t GetFreePosition()
	{
		size_t res = m_FreeIndexes.back();
		m_FreeIndexes.pop_back();
		return res;
	}

	void ReleasePosition(size_t idx)
	{
		m_FreeIndexes.push_back(idx);
	}

	void GetPosition(size_t idx, cocos2d::Vec2 &rPos) const 
	{
		rPos = m_Position +  m_UnitPositions[idx];
	}

	virtual void Update(float dt) = 0;
protected:
	GroupCommander() : m_Position(1500.f, 1500.f) {}

	std::vector<cocos2d::Vec2> m_UnitPositions;
	std::vector<size_t> m_FreeIndexes;
	cocos2d::Vec2 m_Position;
};

class AITriangle : public GroupCommander
{
public:
	AITriangle() ;
	virtual void Update(float dt) override;
	void SetTarget(const BodyBase& enemy) { m_Enemy = &enemy; }
protected:
	const BodyBase* m_Enemy;
};

class TestCommanger : public  AITriangle
{
public:

	static TestCommanger& GetInstace()
	{
		if (!instance)
			instance = new TestCommanger();
		return *instance;
	}

private:
	static TestCommanger *instance;
};



#endif //__AI_GROUP_COMMANDER_H__

