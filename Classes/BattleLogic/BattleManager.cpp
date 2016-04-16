#include "BattleManager.h"

#include "Shaders\Shaders.h"
#include "BattleLogic\Spawner.h"

USING_NS_CC;

BattleManager * BattleManager::m_instance = nullptr;

void BattleManager::initialize(Spawner * spawner)
{
	m_ElapsedTime = 0.0f;
	m_Spawner = spawner;
	///////////////////////////
	//test
	m_Allies.phShips.push_back(new ShipBase(Vec2(1920, 1080), Vec2(1, 0), 400.f, "fighter.png"));
	m_Allies.lShips.push_back(new LogicalShip(1000, 500, 33, 0, new LogicalWeapon()));
	m_Allies.ais.push_back(nullptr);
	m_Allies.phShips[m_PlayerIndex]->Update(0.f);
	m_Allies.phShips[m_PlayerIndex]->GetSprite()->setGLProgram(GetOnHitShader());
}

void BattleManager::free()
{
	////free player
	for (size_t i = 0; i < m_Allies.Size(); ++i)
	{
		delete m_Allies.phShips[i];
		delete m_Allies.lShips[i];
		delete m_Allies.ais[i];
	}
	m_Allies.phShips.resize(0);
	m_Allies.lShips.resize(0);
	m_Allies.ais.resize(0);


	for (size_t i = 0; i < m_PlayerBullets.Size(); ++i)
		delete m_PlayerBullets.bullets[i];
	m_PlayerBullets.bullets.resize(0);
	m_PlayerBullets.weapons.resize(0);

	////free enemy
	m_Enemies.phShips.resize(0);
	m_Enemies.lShips.resize(0);
	m_Enemies.ais.resize(0);


	for (size_t i = 0; i < m_EnemyBullets.Size(); ++i)
	{
		delete m_EnemyBullets.bullets[i];
	}
	m_EnemyBullets.bullets.resize(0);
	m_EnemyBullets.weapons.resize(0);

	//forever tmp
	delete m_Spawner;
}

void BattleManager::SetParent(cocos2d::Layer * parent)
{
	m_ParentLayer = parent;
	setNewParrent();
}

void BattleManager::setNewParrent()
{
	for (size_t i = 0; i < m_Allies.Size(); ++i)
		m_Allies.phShips[i]->SetParent(m_ParentLayer, 0);
	for (size_t i = 0; i < m_PlayerBullets.Size(); ++i)
		m_PlayerBullets.bullets[i]->SetParent(m_ParentLayer, 0);
	for (size_t i = 0; i < m_Enemies.Size(); ++i)
		m_Enemies.phShips[i]->SetParent(m_ParentLayer, 0);
	for (size_t i = 0; i < m_EnemyBullets.Size(); ++i)
		m_EnemyBullets.bullets[i]->SetParent(m_ParentLayer, 0);
}

bool BattleManager::IsThereEnemies() const
{
	//tmp for now, with spawner have to be updated
	return !!(m_Enemies.phShips.size()) || m_Spawner->IsThereEnemies();
}

void BattleManager::Update(const float dt)
{
	if (!m_instance)
		return;

	updateEnemies(dt);
	updatePlayerBullets(dt);
	updateEnemyBullets(dt);
	checkForHitEnemy();
	checkForHitPlayer();
	updatePlayer(dt);

	m_ElapsedTime += dt;


	//checks spwaner for new enemies
	Spawner::SpawnElement tmp;
	if (m_Spawner->GetElementForTime(m_ElapsedTime, tmp))
	{
		tmp.phShip->SetParent(m_ParentLayer, 0);
		m_Enemies.phShips.push_back(tmp.phShip);
		m_Enemies.lShips.push_back(tmp.lShip);
		m_Enemies.ais.push_back(tmp.ai);
	}
}

void BattleManager::updatePlayer(const float dt)
{
	//physical part
	m_Allies.phShips[m_PlayerIndex]->SetDirection(m_PlayerDirCB());
	m_Allies.phShips[m_PlayerIndex]->Update(dt);


	//logical part
	m_Allies.lShips[m_PlayerIndex]->Update(dt);
	m_DisplayPlayerLifeCB(m_Allies.lShips[m_PlayerIndex]->GetLifeInPer());
	m_DisplayPlayerShieldCB(m_Allies.lShips[m_PlayerIndex]->GetShieldInPer());

	//input callbacks
	if (m_PlayerButtonsCB())
		fireBullet(true, m_PlayerIndex, static_cast<UsedSkill>(m_PlayerButtonsCB()));

	//tmp
	UpdateOnHitShader(m_Allies.phShips[m_PlayerIndex]->GetSprite()->getGLProgram(), m_ElapsedTime);
}

void BattleManager::updateEnemies(const float dt)
{
	for (size_t i = 0; i < m_Enemies.Size(); ++i)
	{
		assert(m_Enemies.phShips[i] != nullptr);
		assert(m_Enemies.lShips[i] != nullptr);
		assert(m_Enemies.ais[i] != nullptr);

		//check if death, can death from skilleffect without collision
		if (!m_Enemies.lShips[i]->IsAlive()) {
			auto forDel = m_Enemies.phShips[i];
			m_Enemies.Erase(i);
			startExplosion(forDel);
			break;
		}


		AIMove move = m_Enemies.ais[i]->GetMove(m_Allies.phShips, m_PlayerBullets.bullets, m_Enemies.phShips, dt);
		m_Enemies.phShips[i]->SetDirection(move.newDir);
		m_Enemies.phShips[i]->SetVelocity(move.newVelocity);
		fireBullet(false, i, move.fire);
		reinterpret_cast<EnemyShip*>(m_Enemies.phShips[i])->Update(dt, m_Enemies.lShips[i]->GetLifeInPer(), m_Enemies.lShips[i]->GetShieldInPer());
		m_Enemies.lShips[i]->Update(dt);
	}
}

void BattleManager::updatePlayerBullets(const float dt)
{
	updateBullets(m_PlayerBullets, dt);
}

void BattleManager::updateEnemyBullets(const float dt)
{
	updateBullets(m_EnemyBullets, dt);
}

void BattleManager::updateBullets(Bullets& bulletArray, const float dt)
{
	for (size_t i = 0 ; i < bulletArray.Size(); )
	{
		assert(bulletArray.bullets[i] != nullptr);
		if (!bulletArray.bullets[i]->Update(dt))
		{
			delete bulletArray.bullets[i];
			bulletArray.Erase(i);
		}
		else
			++i;
	}
}

void BattleManager::checkForHitPlayer()
{
	for (size_t i = 0; i < m_EnemyBullets.Size(); ++i)
	{
		for (size_t j = 0; j < m_Allies.Size(); ++j)
		{
			assert(m_EnemyBullets.bullets[i] != nullptr);
			assert(m_Allies.phShips[j] != nullptr);
			if (m_EnemyBullets.bullets[i]->Collision(*(m_Allies.phShips[j])))
			{
				//apply dmg
				m_Allies.lShips[j]->OnHit(m_EnemyBullets.weapons[i]);

				//delete the bullet
				delete m_EnemyBullets.bullets[i];
				m_EnemyBullets.Erase(i);

				//when there is more than one ally do something...

				i--;
				break;
			}
		}
	}
}

void BattleManager::checkForHitEnemy()
{
	bool collision;

	for (size_t i = 0; i < m_PlayerBullets.Size();)
	{
		collision = false;
		for (size_t j = 0; j < m_Enemies.Size();)
		{
			assert(m_PlayerBullets.bullets[i] != nullptr);
			assert(m_Enemies.phShips[j] != nullptr);
			if (m_PlayerBullets.bullets[i]->Collision(*(m_Enemies.phShips[j])))
			{

				collision = true;
				//apply dmg
				m_Enemies.lShips[j]->OnHit(m_PlayerBullets.weapons[i]);
				reinterpret_cast<EnemyShip*>(m_Enemies.phShips[j])->UpdateHPBars(m_Enemies.lShips[j]->GetLifeInPer(), m_Enemies.lShips[j]->GetShieldInPer());

				//delete the bullet
				delete m_PlayerBullets.bullets[i];
				m_PlayerBullets.Erase(i);

				//if enemy dies explosion and delete it
				if (!m_Enemies.lShips[j]->IsAlive()) {
					auto forDel = m_Enemies.phShips[j];
					m_Enemies.Erase(j);
					startExplosion(forDel);
				}
				break;
			}
			else
				++j;
		}
		if (!collision)
			++i;
	}
}

void BattleManager::fireBullet(bool isPlayerBullet, size_t shooterIdx, UsedSkill usedSkill)
{
	if (usedSkill == UsedSkill::None) return;
	const Ships& crn = isPlayerBullet ? m_Allies : m_Enemies;
	assert(crn.lShips[shooterIdx] != nullptr);
	assert(crn.phShips[shooterIdx] != nullptr);

	auto lShip = crn.lShips[shooterIdx];
	const auto phShip = crn.phShips[shooterIdx];
	
	auto res = lShip->GetWeapon()->Cast(usedSkill, phShip->GetPosition(), phShip->GetDirection());
	if (res.m_Bullet)
	{
		auto& bulletArray = isPlayerBullet ? m_PlayerBullets : m_EnemyBullets;
		bulletArray.bullets.push_back(res.m_Bullet);
		bulletArray.weapons.push_back(res.m_Source);
		bulletArray.bullets.back()->SetParent(m_ParentLayer, 0);
	}
	else if (res.m_Type == SkillResult::ResultType::Effect)
	{
		lShip->AddEffect(res.m_Effect);
	}
}

void BattleManager::loadExplosionAnim()
{
	SpriteBatchNode* spritebatch = SpriteBatchNode::create("ShipExplosion/Explosion.png");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("ShipExplosion/Explosion.plist");
	char str[100] = { 0 };
	for (int i = 0; i < 24; i++)
	{
		sprintf(str, "Explosion/%i.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		m_ExplosionAnim.pushBack(frame);
	}
}

void BattleManager::startExplosion(ShipBase * ship)
{
	auto animation = Animation::createWithSpriteFrames(m_ExplosionAnim, 0.03f);
	auto animate = Animate::create(animation);
	auto seq = Sequence::create(animate, CallFunc::create([ship]() { ship->SetVisible(false); }), NULL);
	ship->GetSprite()->runAction(seq);
}
