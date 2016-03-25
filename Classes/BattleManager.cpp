#include "BattleManager.h"

USING_NS_CC;

BattleManager * BattleManager::m_instance = nullptr;

void BattleManager::initialize()
{
	///////////////////////////
	//test
	m_Allies.phShips.push_back(new ShipBase(Vec2(1920, 1080), Vec2(1, 0), 10.f, "fighter.png"));
	m_Allies.lShips.push_back(new LogicalShip(1000, 500, 33, 0, new LogicalWeapon(200.f, 0.18f)));
	m_Allies.ais.push_back(nullptr);
	m_Allies.phShips[m_PlayerIndex]->Update();
			
	ShipBase * enemy = new ShipBase(Vec2(1920, 540), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	LogicalShip * lEnemy = new LogicalShip(1000, 700, 33, 0, new LogicalWeapon(100.f, 1.f));
	AIPointToPoint * aiEnemyp = new AIPointToPoint(*enemy, *(lEnemy->GetWeapon()));
	aiEnemyp->AddPoint(cocos2d::Vec2(200, 200));
	aiEnemyp->AddPoint(cocos2d::Vec2(2200, 1200));
	m_Enemies.phShips.push_back(enemy);
	m_Enemies.lShips.push_back(lEnemy);
	m_Enemies.ais.push_back(aiEnemyp);
	
	AIBase *aiEnemy;
	enemy = new ShipBase(Vec2(960, 1080), Vec2(0, 0), 10.f, "enemies/Enemy4.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	aiEnemy = new AICoward(*enemy, *(lEnemy->GetWeapon()), 250, 400);
	m_Enemies.phShips.push_back(enemy);
	m_Enemies.lShips.push_back(lEnemy);
	m_Enemies.ais.push_back(aiEnemy);
	enemy = new ShipBase(Vec2(960, 1620), Vec2(0, 0), 10.f, "enemies/Enemy3.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	aiEnemy = new AICoward(*enemy, *(lEnemy->GetWeapon()), 250, 400);
	m_Enemies.phShips.push_back(enemy);
	m_Enemies.lShips.push_back(lEnemy);
	m_Enemies.ais.push_back(aiEnemy);
	enemy = new ShipBase(Vec2(960, 540), Vec2(0, 0), 10.f, "enemies/Enemy6.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	aiEnemy = new AICoward(*enemy, *(lEnemy->GetWeapon()), 250, 400);
	m_Enemies.phShips.push_back(enemy);
	m_Enemies.lShips.push_back(lEnemy);
	m_Enemies.ais.push_back(aiEnemy);
	enemy = new ShipBase(Vec2(2780, 1620), Vec2(0, 0), 10.f, "enemies/Enemy1.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	aiEnemy = new AIBase(*enemy, *(lEnemy->GetWeapon()));
	m_Enemies.phShips.push_back(enemy);
	m_Enemies.lShips.push_back(lEnemy);
	m_Enemies.ais.push_back(aiEnemy);
	enemy = new ShipBase(Vec2(1920, 1620), Vec2(0, 0), 10.f, "enemies/Enemy2.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	aiEnemy = new AIBase(*enemy, *(lEnemy->GetWeapon()));
	m_Enemies.phShips.push_back(enemy);
	m_Enemies.lShips.push_back(lEnemy);
	m_Enemies.ais.push_back(aiEnemy);
	enemy = new ShipBase(Vec2(2780, 1080), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	aiEnemy = new AIBase(*enemy, *(lEnemy->GetWeapon()));
	m_Enemies.phShips.push_back(enemy);
	m_Enemies.lShips.push_back(lEnemy);
	m_Enemies.ais.push_back(aiEnemy);
	
	///////////////////////////
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
	for (size_t i = 0; i < m_Enemies.Size(); ++i)
	{
		delete m_Enemies.phShips[i];
		delete m_Enemies.lShips[i];
		delete m_Enemies.ais[i];
	}
	m_Enemies.phShips.resize(0);
	m_Enemies.lShips.resize(0);
	m_Enemies.ais.resize(0);


	for (size_t i = 0; i < m_EnemyBullets.Size(); ++i)
	{
		delete m_EnemyBullets.bullets[i];
	}
	m_EnemyBullets.bullets.resize(0);
	m_EnemyBullets.weapons.resize(0);
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

bool BattleManager::IsThereEnemies()
{
	//tmp for now, with spawner have to be updated
	return m_Enemies.phShips.size();
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
}

void BattleManager::updatePlayer(const float dt)
{
	//physical part
	m_Allies.phShips[m_PlayerIndex]->SetDirection(m_PlayerDirCB());
	m_Allies.phShips[m_PlayerIndex]->Update();

	//logical part
	m_Allies.lShips[m_PlayerIndex]->Update(dt);
	m_DisplayPlayerLifeCB(m_Allies.lShips[m_PlayerIndex]->GetLifeInPer());
	m_DisplayPlayerShieldCB(m_Allies.lShips[m_PlayerIndex]->GetShieldInPer());

	//input callbacks
	if ((m_PlayerButtonsCB() & 1) == 1)
		fireBullet(true, m_PlayerIndex);
}

void BattleManager::updateEnemies(const float dt)
{
	for (size_t i = 0; i < m_Enemies.Size(); ++i)
	{
		AIMove move = m_Enemies.ais[i]->GetMove(m_Allies.phShips, m_PlayerBullets.bullets, m_Enemies.phShips);
		m_Enemies.phShips[i]->SetDirection(move.newDir);
		m_Enemies.phShips[i]->SetVelocity(move.newVelocity);
		if (move.fire == AIMove::FireType::NormalAttack)
			fireBullet(false, i);
		m_Enemies.phShips[i]->Update();
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
		if (!bulletArray.bullets[i]->Update())
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
			if (m_EnemyBullets.bullets[i]->Collision(*(m_Allies.phShips[j])))
			{
				//apply dmg
				m_Allies.lShips[j]->OnHit(m_EnemyBullets.weapons[i]);

				//delete the bullet
				delete m_EnemyBullets.bullets[i];
				m_EnemyBullets.Erase(i);

				//when there is more than one ally do something...
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
	BulletBase * bullet = new BulletBase(crn.phShips[shooterIdx]->GetPosition(), crn.phShips[shooterIdx]->GetDirection().getNormalized(), 25.f, spriteNames[isPlayerBullet]);
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
	auto seq = Sequence::create(animate, animate->reverse(), CallFunc::create([ship]() { delete ship; }), NULL);
	ship->GetSprite()->runAction(seq);
}
