#ifndef  __GAME_MANAGER_H__
#define  __GAME_MANAGER_H__

#include "cocos2d.h"

class GameManager
{
	static GameManager * m_instance;
	GameManager() {}
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
private:

};

#endif // __GAME_MANAGER_H__
