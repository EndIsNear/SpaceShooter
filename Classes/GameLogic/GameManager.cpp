#include "GameManager.h"

USING_NS_CC;

GameManager * GameManager::m_instance = nullptr;

void GameManager::Initialize()
{
	//TODO: read all of this from file
	m_LogicalShipsDB.push_back(new LogicalShip(300, 100, 33, 0));
	m_LogicalShipsDB.push_back(new LogicalShip(600, 400, 33, 0));
	m_LogicalShipsDB.push_back(new LogicalShip(1000, 700, 33, 0));

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

