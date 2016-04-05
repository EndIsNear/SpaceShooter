#include "GameManager.h"
#include "Utils\FileReader.h"

USING_NS_CC;

GameManager * GameManager::m_instance = nullptr;

void GameManager::Initialize()
{
	m_CrnLevel = 0;
	loadShips();
	loadWeapons();
	loadLevels();

	char fileName[128];
	for (int i = 1; i <= 6; ++i)
	{
		sprintf(fileName, "enemies/Enemy%d.png", i);
		m_SpriteNamesDB.push_back(fileName);
	}
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

size_t GameManager::GetLevelCount() const
{
	return m_LevelInfos.size();
}


void GameManager::loadShips()
{
	FileReader<LogicalShip> shipsFile("jsons/ships.json");
	unsigned shipCnt = shipsFile.GetEntryCount();
	m_LogicalShipsDB.resize(shipCnt);
	for (unsigned i = 0; i < shipCnt; i++)
	{
		m_LogicalShipsDB[i] = new LogicalShip;
		shipsFile.GetEntryByIndex(i, *m_LogicalShipsDB[i]);
	}
}

void GameManager::loadWeapons()
{
	FileReader<LogicalWeapon> wepsFile("jsons/weapons.json");
	unsigned wepCnt = wepsFile.GetEntryCount();
	m_LogicalWeaponsDB.resize(wepCnt);
	for (unsigned i = 0; i < wepCnt; i++)
	{
		m_LogicalWeaponsDB[i] = new LogicalWeapon;
		wepsFile.GetEntryByIndex(i, *m_LogicalWeaponsDB[i]);
	}
}

void GameManager::loadLevels()
{
	FileReader<LevelInfo> levelsFile("jsons/levels.json");
	unsigned lvlCnt = levelsFile.GetEntryCount();
	m_LevelInfos.resize(lvlCnt);
	for (unsigned i = 0; i < lvlCnt; i++)
	{
		levelsFile.GetEntryByIndex(i, m_LevelInfos[i]);
	}
}

