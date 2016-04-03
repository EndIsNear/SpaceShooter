#ifndef  __LEVEL_INFO_H__
#define  __LEVEL_INFO_H__

#include "cocos2d.h"
#include "external\json\document.h"

struct LevelInfo
{
	struct SpawnEntry
	{
		size_t spwnPntID;
		size_t lShipID;
		size_t lWeaponID;
		size_t aiID;
		size_t sprtNameID;
		float timeToSpawn;
		bool Load(const rapidjson::GenericValue<rapidjson::UTF8<> > &entry);
	};
	//spawn pts, pair of position and ID
	typedef std::pair<cocos2d::Vec2, size_t> SpawnPointInfo;


	cocos2d::Size m_LevelSize;
	cocos2d::Vec2 m_PlayerStartPos;

	
	std::vector<SpawnPointInfo> m_SpawnPoints;
	std::vector<SpawnEntry> m_SpawnEntries;

	bool Load(const rapidjson::GenericValue<rapidjson::UTF8<> > &entry);
};

#endif // __LEVEL_INFO_H__
