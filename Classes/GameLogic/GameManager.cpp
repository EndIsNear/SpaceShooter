#include "GameManager.h"

USING_NS_CC;

GameManager * GameManager::m_instance = nullptr;

void GameManager::Initialize()
{
	//TODO: read all of this from file
	m_LogicalShipsDB.push_back(new LogicalShip(300, 100, 33, 0));
	m_LogicalShipsDB.push_back(new LogicalShip(600, 400, 33, 0));
	m_LogicalShipsDB.push_back(new LogicalShip(1000, 700, 33, 0));

	m_LogicalWeaponsDB.push_back(new LogicalWeapon(50.f, 0.4f, 700.f));
	m_LogicalWeaponsDB.push_back(new LogicalWeapon(70.f, 0.6f, 700.f));
	m_LogicalWeaponsDB.push_back(new LogicalWeapon(100.f, 1.f, 700.f));

	char fileName[128];
	for (int i = 1; i <= 6; ++i)
	{
		sprintf(fileName, "enemies/Enemy%d.png", i);
		m_SpriteNamesDB.push_back(fileName);
	}


	LevelInfo tmp;
	tmp.m_SpawnPoints.emplace_back(Vec2(1920, 540), 0);
	for (int i = 0; i < 5; ++i)
	{
		LevelInfo::SpawnEntry tmpEntry;
		tmpEntry.timeToSpawn = i * 2.f;
		tmpEntry.lShipID = i % 3;
		tmpEntry.lWeaponID = i % 3;
		tmpEntry.spwnPntID = 0;
		tmpEntry.sprtNameID = i;
		tmp.m_SpawnEntries.push_back(tmpEntry);
	}

	m_LevelInfos.push_back(tmp);
}

const std::vector<LogicalShip*>& GameManager::GetLogicalShips() const
{
	return m_LogicalShipsDB;
}

const std::vector<LogicalWeapon*>& GameManager::GetLogicalWeapons() const
{
	return m_LogicalWeaponsDB;
}

const std::vector<std::string>& GameManager::GetSpriteNames() const
{
	return m_SpriteNamesDB;
}

const LevelInfo& GameManager::GetLevelInfo(size_t levelID) const
{
	assert(levelID <= m_LevelInfos.size());
	return m_LevelInfos[levelID];
}

