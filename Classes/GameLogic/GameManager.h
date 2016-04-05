#ifndef  __GAME_MANAGER_H__
#define  __GAME_MANAGER_H__

#include "cocos2d.h"
#include "BattleLogic\LogicalShip.h"
#include "LevelInfo.h"

class GameManager
{
	static GameManager * m_instance;
	GameManager() { Initialize(); }
	~GameManager() {}
	GameManager(const GameManager&) = delete;
	GameManager& operator= (const GameManager&) = delete;
public:
	static GameManager * Instance()
	{
		if (!m_instance)
			m_instance = new GameManager;
		return m_instance;
	}

	static void ReleaseInstance()
	{
		if (m_instance)
			delete m_instance;
		m_instance = nullptr;
	}
	//end of singleton part
	//////////////////////////////
	const std::vector<LogicalShip*>& GetLogicalShips() const;
	const std::vector<LogicalWeapon*>& GetLogicalWeapons() const;
	const std::vector<std::string>& GetSpriteNames() const;
	const LevelInfo& GetLevelInfo(size_t levelID) const;
	size_t GetLevelCount() const;

	size_t GetCrnLevel() const { return m_CrnLevel; }
	void SetCrnLevel(size_t n) { m_CrnLevel = n; }

	void Initialize();

private:
	std::vector<LogicalShip*>   m_LogicalShipsDB;
	std::vector<LogicalWeapon*> m_LogicalWeaponsDB;
	std::vector<std::string>    m_SpriteNamesDB;
	std::vector<LevelInfo>      m_LevelInfos;

	size_t m_CrnLevel;

	void loadShips();
	void loadWeapons();
	void loadLevels();
};

#endif // __GAME_MANAGER_H__
