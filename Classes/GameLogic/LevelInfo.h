#ifndef  __LEVEL_INFO_H__
#define  __LEVEL_INFO_H__

#include "cocos2d.h"

struct SpawnerInfo
{
	struct SpawnEntry
	{
		size_t spwnPntID;
		size_t lShipID;
		size_t aiID;
		size_t sprtNameID;
		float timeToSpawn;
	};

	cocos2d::Vec2 m_PlayerStartPos;
	//spawn pts, pair of position and ID
	std::vector<std::pair<cocos2d::Vec2, size_t>> m_SpawnPoints;
	std::vector<SpawnEntry> m_SpawnEntries;
};

#endif // __LEVEL_INFO_H__
