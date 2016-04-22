#ifndef  __BATTLE_MANAGER_H__
#define  __BATTLE_MANAGER_H__

#include "cocos2d.h"
#include "Physics\BodyBase.h"
#include "Physics\ShipBase.h"
#include "Physics\BulletBase.h"
#include "BattleLogic\Spawner.h"
#include "AI\AIBase.h"
#include "Utils.h"

class BattleManager
{
	static BattleManager * m_instance;
	BattleManager() { loadExplosionAnim(); }
	~BattleManager() {}	BattleManager(const BattleManager&) = delete;
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

	//end of singleton part
	//////////////////////////////
	void initialize(Spawner * spawner);
	void free();

	float GetElapsedTime() { return m_ElapsedTime; }
	void SetParent(cocos2d::Layer * parent);
	void Update(const float dt);
	bool IsPlayerAlive() const { return m_Allies.lShips[0]->IsAlive(); }
	//returns all player cooldowns in percents
	bool GetPlayerCooldowns(float(&cdns)[4]);
	bool IsThereEnemies() const;
	const BodyBase * ptrShip() const { return m_Allies.phShips[0]; }


	void setPlayerDirCallback(std::function<cocos2d::Vec2()> _f) { m_PlayerDirCB = _f; }
	void setPlayerButtonCallback(std::function<unsigned()> _f) { m_PlayerButtonsCB = _f; }
	void setPlayerLifeDispCallback(std::function<void(float)> _f) { m_DisplayPlayerLifeCB = _f; }
	void setPlayerShieldDispCallback(std::function<void(float)> _f) { m_DisplayPlayerShieldCB = _f; }
private:
	const size_t m_PlayerIndex = 0;
	//private structures
	struct Ships
	{
		inline size_t Size() const
		{
			assert(phShips.size() == lShips.size());
			assert(lShips.size() == ais.size());
			return phShips.size();
		}

		inline void Erase(size_t idx)
		{
			const size_t size = Size();
			assert(idx < size);
			if (idx >= size)
				return;
			phShips[idx] = phShips[size - 1];
			lShips[idx] = lShips[size - 1];
			ais[idx] = ais[size - 1];
			phShips.pop_back();
			lShips.pop_back();
			ais.pop_back();
		}

		//player on idx 0 always for allies
		std::vector<ShipBase*> phShips;///< physical part of ship
		std::vector<LogicalShip*> lShips;///< logical part of ship
		std::vector<AIBaseInterface*> ais;
	};

	struct Bullets
	{
		inline size_t Size() const
		{
			assert(bullets.size() == skills.size());
			return bullets.size();
		}

		inline void Erase(size_t idx)
		{
			const size_t size = Size();
			assert(idx < size);
			if (idx >= size)
				return;
			bullets[idx] = bullets[size - 1];
			skills[idx] = skills[size - 1];
			bullets.pop_back();
			skills.pop_back();
		}
		std::vector<BulletBase*> bullets;
		std::vector<SkillInterface*> skills; ///< ptr that contains the shooter, used to calc dmg
	};


	void setNewParrent();
	void fireBullet(bool isPlayerBullet, size_t shooterIdx, UsedSkill usedSkill);
	void startExplosion(ShipBase * ship);


	//updates all of this should be called every frame
	void updatePlayer(const float dt);
	void updateEnemies(const float dt);
	void updatePlayerBullets(const float dt);
	void updateEnemyBullets(const float dt);
	void updateBullets(Bullets& bulletArray, const float dt);
	void checkForHitPlayer();
	void checkForHitEnemy();

	void loadExplosionAnim();
	void applySkillBulletsResult(bool playerBullets, SkillResult& res);

	std::function<cocos2d::Vec2()> m_PlayerDirCB;
	std::function<unsigned()> m_PlayerButtonsCB; ///< result is bitfield for pressed buttons
	std::function<void(float)> m_DisplayPlayerLifeCB;
	std::function<void(float)> m_DisplayPlayerShieldCB;

	cocos2d::Layer * m_ParentLayer;
	//should be deleted from this class on restart
	Spawner * m_Spawner;

	Ships m_Allies;
	Ships m_Enemies;
	Bullets m_PlayerBullets;
	Bullets m_EnemyBullets;
	float m_ElapsedTime;

	cocos2d::GLProgramState *m_state;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_ExplosionAnim;
};

#endif // __BATTLE_MANAGER_H__
