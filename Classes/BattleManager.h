#ifndef  __BATTLE_MANAGER_H__
#define  __BATTLE_MANAGER_H__

#include "cocos2d.h"
#include "Physics\BodyBase.h"
#include "Physics\ShipBase.h"
#include "Physics\BulletBase.h"
#include "BattleLogic\Spawner.h"

const float timeBetweenShootings = 0.5f;


//struct InitialBattleStats
//{
	//player ship weapon
	//ligic ship
	//sprite name
	//pos

	//spawner
	//vector<pos> spawning pts
	//vector<ship> enemtyType
	//vector<ai> aiType

	//struct spPnt, enemyType, aiType, timeFromBeg
//};

class BattleManager
{
	static BattleManager * m_instance;
	BattleManager(const BattleManager&) = delete;
	BattleManager& operator= (const BattleManager&) = delete;
public:
	static BattleManager * Instance()
	{
		if (!m_instance)
			m_instance = new BattleManager;
		return m_instance;
	}

	static void ReleaseInstance()
	{
		if (m_instance)
			delete m_instance;
		m_instance = nullptr;
	}
	//////////////////////////////
	//////////////////////////////
	//////////////////////////////
	void initialize();
	void free();

	void SetParent(cocos2d::Layer * parent);
	void Update(const float dt);
	bool IsPlayerAlive() { return m_Player.lShip->IsAlive(); }
	BodyBase * ptrShip() { return m_Player.phShip; }


	void setPlayerDirCallback(std::function<cocos2d::Vec2()> _f) { m_PlayerDirCB = _f; }
	void setPlayerButtonCallback(std::function<unsigned()> _f) { m_PlayerButtonsCB = _f; }
	void setPlayerLifeDispCallback(std::function<void(float)> _f) { m_DisplayPlayerLifeCB = _f; }
	void setPlayerShieldDispCallback(std::function<void(float)> _f) { m_DisplayPlayerShieldCB = _f; }
private:
	//private structures
	struct Player
	{
		LogicalShip * lShip;///< logical part of ship
		ShipBase * phShip;///< physical part of ship
	};

	struct Enemy : public Player
	{
		Enemy() {}
		Enemy(AI _ai, Player _p) : Player(_p), ai(_ai) {}
		AI ai;
	};

	struct Bullet
	{
		LogicalWeapon * lWeapon; ///< ptr that contains the shooter, used to calc dmg
		BulletBase * phBullet;
	};

	BattleManager() {}
	~BattleManager() {}

	void setNewParrent();
	void fireBullet(bool isPlayerBullet, Player * shooter);
	void startExplosion(ShipBase * ship);


	//updates all of this should be called every frame
	void updatePlayer(const float dt);
	void updateEnemies(const float dt);
	void updatePlayerBullets(const float dt);
	void updateEnemyBullets(const float dt);
	void updateBullets(std::vector<BulletBase*>& bulletArray, const float dt);
	void checkForHitPlayer();
	void checkForHitEnemy();

	std::function<cocos2d::Vec2()> m_PlayerDirCB;
	std::function<unsigned()> m_PlayerButtonsCB; ///< result is bitfield for pressed buttons
	std::function<void(float)> m_DisplayPlayerLifeCB;
	std::function<void(float)> m_DisplayPlayerShieldCB;

	cocos2d::Layer * m_ParentLayer;
	//should be deleted from this class on restart
	Spawner * m_Spawner;

	Player m_Player;
	std::vector<BulletBase*> m_PlayerBullets;
	std::vector<Enemy> m_Enemies;
	std::vector<BulletBase*> m_EnemyBullets;
};

#endif // __BATTLE_MANAGER_H__
