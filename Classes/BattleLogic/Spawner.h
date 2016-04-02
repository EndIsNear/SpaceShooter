#ifndef  __SPAWNER_H__
#define  __SPAWNER_H__

#include <cocos2d.h>
#include <vector>
#include "LogicalShip.h"
#include "AI\AIBase.h"
#include "Physics\ShipBase.h"
#include "Physics\BulletBase.h"

struct Spawner
{
	Spawner() : m_crnIdx(0) { Initialize(); }
	
	struct SpawnElement
	{
		LogicalShip * lShip;
		ShipBase * phShip;
		AIBaseInterface * ai;
	};

	//std::vector<cocos2d::Vec2*> m_SpawningPts;//spawning pts
	//std::vector<LogicalShip*> m_ShipTypes;
	//std::vector<AIBase*> m_AITypes;
	//std::vector<SpawnElement> m_SpawnElements;

	bool GetElementForTime(const float time, SpawnElement& res);
	bool IsThereEnemies() { return m_crnIdx < m_SpawnElements.size(); }
private:
	void Initialize();

	size_t m_crnIdx;
	std::vector <std::pair<float, SpawnElement>> m_SpawnElements;
};

#endif // __SPAWNER_H__
