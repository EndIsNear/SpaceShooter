#include "LevelInfo.h"

using namespace rapidjson;

bool LevelInfo::SpawnEntry::Load(const GenericValue<UTF8<> > &entry)
{
	spwnPntID = entry["spwnPntID"].GetInt();
	lShipID = entry["lShipID"].GetInt();
	lWeaponID = entry["lWeaponID"].GetInt();
	aiID = entry["aiID"].GetInt();
	sprtNameID = entry["sprtNameID"].GetInt();
	shipSpeed = entry["shipSpeed"].GetDouble();
	return true;
}

bool LevelInfo::Load(const GenericValue<UTF8<>> &entry)
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
		const GenericValue<UTF8<> >& SpawnEntriesArr = entry["SpawnEnties"];
		SizeType spawnEntriesCnt = SpawnEntriesArr.Size();
		unsigned realShipCnt = 0;
		for (SizeType i = 0; i < spawnEntriesCnt; i++)
		{
			// count real ships
			realShipCnt += SpawnEntriesArr[i]["timeToSpawn"].Size();
		}
		m_SpawnEntries.resize(realShipCnt);
		for (SizeType i = 0; i < spawnEntriesCnt; i++)
		{
			m_SpawnEntries[i].Load(SpawnEntriesArr[i]);
			const GenericValue<UTF8<> >& TimeToSwapnArr = SpawnEntriesArr[i]["timeToSpawn"];
			m_SpawnEntries[i].timeToSpawn = TimeToSwapnArr[0].GetDouble();
			SizeType otherEntriesCnt = TimeToSwapnArr.Size();
			for (SizeType j = 1; j < otherEntriesCnt; j++)
			{
				m_SpawnEntries[i + j] = m_SpawnEntries[i];
				m_SpawnEntries[i + j].timeToSpawn = TimeToSwapnArr[j].GetDouble();
			}
			i += otherEntriesCnt - 1;
		}
	}

	// read spawn points
	{
		const GenericValue<UTF8<> >& SpawnPtsArr = entry["SpawnPoints"];
		SizeType spawnPtsCnt = SpawnPtsArr.Size();
		m_SpawnPoints.resize(spawnPtsCnt);
		for (SizeType i = 0; i < spawnPtsCnt; i++)
		{
			SpawnPointInfo& currenPt = m_SpawnPoints[i];
			const GenericValue<UTF8<> >& currentEntry = SpawnPtsArr[i];
			currenPt.first = cocos2d::Vec2(currentEntry["X"].GetInt(), currentEntry["Y"].GetInt());
			currenPt.second = currentEntry["SpawnEntryID"].GetInt();
		}
	}

	return true;
}