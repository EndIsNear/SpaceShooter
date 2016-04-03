#include "LevelInfo.h"

using namespace rapidjson;

bool LevelInfo::SpawnEntry::Load(const GenericValue<UTF8<> > &entry)
{
	spwnPntID = entry["spwnPntID"].GetInt();
	lShipID = entry["lShipID"].GetInt();
	lWeaponID = entry["lWeaponID"].GetInt();
	aiID = entry["aiID"].GetInt();
	sprtNameID = entry["sprtNameID"].GetInt();
	timeToSpawn = entry["timeToSpawn"].GetDouble();
	return true;
}

bool LevelInfo::Load(const GenericValue<UTF8<> > &entry)
{

	// read level size
	float h = entry["LevelH"].GetInt(); // level higth in px
	float w = entry["LevelW"].GetInt(); // level width in px
	m_LevelSize = cocos2d::Size(h, w);

	// read player position
	float x = entry["PlayerX"].GetInt(); //  in px
	float y = entry["PlayerY"].GetInt(); //  in px
	m_PlayerStartPos = cocos2d::Size(x,y);

	// read all spawn entryes
	{
		const GenericValue<UTF8<> > &SwapnEntriesArr = entry["SpawnEnties"];
		SizeType spawnEntriesCnt = SwapnEntriesArr.Size();
		m_SpawnEntries.resize(spawnEntriesCnt);
		for (SizeType i = 0; i < spawnEntriesCnt; i++)
		{
			m_SpawnEntries[i].Load(SwapnEntriesArr[i]);
		}
	}

	// read spawn points
	{
		const GenericValue<UTF8<> > &SwapnPtsArr = entry["SpawnPoints"];
		SizeType spawnPtsCnt = SwapnPtsArr.Size();
		m_SpawnPoints.resize(spawnPtsCnt);
		for (SizeType i = 0; i < spawnPtsCnt; i++)
		{
			SpawnPointInfo& currenPt = m_SpawnPoints[i];
			const GenericValue<UTF8<> >& currentEntry = SwapnPtsArr[i];
			currenPt.first = cocos2d::Vec2(currentEntry["X"].GetInt(), currentEntry["Y"].GetInt());
			currenPt.second = currentEntry["SpawnEntryID"].GetInt();
		}
	}

	return true;
}