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
	Spawner() : m_crnIdx(0)
	{ 
		//tmp should be created with level ID
		Initialize(0);
	}
	
	struct SpawnElement
	{
		LogicalShip * lShip;
		ShipBase * phShip;
		AIBaseInterface * ai;
	};

	bool GetElementForTime(const float time, SpawnElement& res);
	bool IsThereEnemies() { return m_crnIdx < m_SpawnElements.size(); }
private:
	void Initialize(size_t levelID);

	size_t m_crnIdx;
	std::vector <std::pair<float, SpawnElement>> m_SpawnElements;
};

#endif // __SPAWNER_H__
