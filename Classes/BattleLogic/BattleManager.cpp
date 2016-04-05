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
	m_Allies.lShips.push_back(new LogicalShip(1000, 500, 33, 0, new LogicalWeapon(200.f, 0.18f, 800.f)));
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
	if ((m_PlayerButtonsCB() & 1) == 1)
		fireBullet(true, m_PlayerIndex);

	//tmp
	UpdateOnHitShader(m_Allies.phShips[m_PlayerIndex]->GetSprite()->getGLProgram(), m_ElapsedTime);
}

void BattleManager::updateEnemies(const float dt)
{
	for (size_t i = 0; i < m_Enemies.Size(); ++i)
	{
		AIMove move = m_Enemies.ais[i]->GetMove(m_Allies.phShips, m_PlayerBullets.bullets, m_Enemies.phShips, dt);
		m_Enemies.phShips[i]->SetDirection(move.newDir);
		m_Enemies.phShips[i]->SetVelocity(move.newVelocity);
		if (move.fire == AIMove::FireType::NormalAttack)
			fireBullet(false, i);
		m_Enemies.phShips[i]->Update(dt);
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
			if (m_PlayerBullets.bullets[i]->Collision(*(m_Enemies.phShips[j])))
			{
				collision = true;
				//apply dmg
				m_Enemies.lShips[j]->OnHit(m_PlayerBullets.weapons[i]);

				//delete the bullet
				delete m_PlayerBullets.bullets[i];
				m_PlayerBullets.Erase(i);

				//if enemy dies explosion and delete it
				if (!m_Enemies.lShips[j]->IsAlive()) {
					//TODO: lShip and weapon leaks
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

void BattleManager::fireBullet(bool isPlayerBullet, size_t shooterIdx)
{
	const Ships& crn = isPlayerBullet ? m_Allies : m_Enemies;
	if (!crn.lShips[shooterIdx]->GetWeapon()->CanShoot())
		return;

	crn.lShips[shooterIdx]->GetWeapon()->Shoot();
	const char * spriteNames[2] = {"EnemyBullet.png", "bullet.png"};
	BulletBase * bullet = new BulletBase(crn.phShips[shooterIdx]->GetPosition(), crn.phShips[shooterIdx]->GetDirection().getNormalized(), crn.lShips[shooterIdx]->GetWeapon()->GetBulletSpeed(), spriteNames[isPlayerBullet]);
	auto& bulletArray = isPlayerBullet ? m_PlayerBullets : m_EnemyBullets;
	bulletArray.bullets.push_back(bullet);
	bulletArray.weapons.push_back(crn.lShips[shooterIdx]->GetWeapon());
	
	bulletArray.bullets.back()->SetParent(m_ParentLayer, 0);
}

void BattleManager::startExplosion(ShipBase * ship)
{
	SpriteBatchNode* spritebatch = SpriteBatchNode::create("ShipExplosion/Plist.png");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("ShipExplosion/Plist.plist");


	Vector<SpriteFrame*> animFrames;
	char str[100] = { 0 };
	for (int i = 1; i < 6; i++)
	{
		sprintf(str, "ShipExplosion/Explosion_%01d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	auto animate = Animate::create(animation);
	auto seq = Sequence::create(animate, animate->reverse(), CallFunc::create([ship]() { ship->GetSprite()->setVisible(false); }), NULL);
	ship->GetSprite()->runAction(seq);
}
