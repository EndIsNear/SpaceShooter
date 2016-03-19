#ifndef  __SPAWNER_H__
#define  __SPAWNER_H__

#include <cocos2d.h>
#include <vector>
#include "LogicalShip.h"
#include "Physics\ShipBase.h"
#include "Physics\BulletBase.h"

struct AIMove
{
	int fire;
	cocos2d::Vec2 newDir;
	float newVelocity;
};

typedef std::function<AIMove(ShipBase *, std::vector<BulletBase*>, ShipBase *)> AI;

struct Spawner
{
	Spawner() : m_crnIdx(0) {}
	struct SpawnElement
	{
		int pnt;
		int shipType;
		int AIType;
		float time;
	};
	std::vector<cocos2d::Vec2> m_SpawningPts;//spawning pts
	std::vector<LogicalShip> m_ShipTypes;
	std::vector<AI> m_AITypes;
	std::vector<SpawnElement> m_SpawnElements;
	bool getElementForTime(const float time, SpawnElement& res);
private:
	int m_crnIdx;
};

#endif // __SPAWNER_H__
