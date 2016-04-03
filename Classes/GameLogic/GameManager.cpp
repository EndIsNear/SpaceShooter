#include "GameManager.h"
#include "Utils\FileReader.h"

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


	FileReader<LevelInfo> test("jsons/levels.json");
	unsigned lvlCnt = test.GetEntryCount();
	m_LevelInfos.resize(lvlCnt);
	for (unsigned i = 0; i < lvlCnt; i++)
	{
		test.GetEntryByIndex(i, m_LevelInfos[i]);
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

