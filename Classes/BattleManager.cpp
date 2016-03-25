#include "BattleManager.h"
#include <random>

USING_NS_CC;

BattleManager * BattleManager::m_instance = nullptr;



void BattleManager::initialize()
{
	///////////////////////////
	//test
	m_Player.phShip = new ShipBase(Vec2(1920, 1080), Vec2(1, 0), 10.f, "fighter.png");
	m_Player.lShip = new LogicalShip(1000, 500, 33, 0, new LogicalWeapon(200.f, 0.18f));
	m_Player.phShip->Update();

			
	ShipBase * enemy = new ShipBase(Vec2(1920, 540), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	LogicalShip * lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(960, 1080), Vec2(0, 0), 10.f, "enemies/Enemy4.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(960, 1620), Vec2(0, 0), 10.f, "enemies/Enemy3.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(960, 540), Vec2(0, 0), 10.f, "enemies/Enemy6.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(2780, 1620), Vec2(0, 0), 10.f, "enemies/Enemy1.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(1920, 1620), Vec2(0, 0), 10.f, "enemies/Enemy2.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(2780, 1080), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	lEnemy = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));

	///////////////////////////
}

void BattleManager::free()
{
	//free player
	delete m_Player.phShip;
	delete m_Player.lShip;
	for (auto& bullet : m_PlayerBullets)
		delete bullet.phBullet;
	m_PlayerBullets.resize(0);

	//free enemy
	for (auto& enemy : m_Enemies)
	{
		delete enemy.phShip;
		delete enemy.lShip->GetWeapon();
		delete enemy.lShip;
	}
	m_Enemies.resize(0);
	for (auto& bullet : m_EnemyBullets)
		delete bullet.phBullet;
	m_EnemyBullets.resize(0);
}

void BattleManager::SetParent(cocos2d::Layer * parent)
{
	m_ParentLayer = parent;
	setNewParrent();
}

void BattleManager::setNewParrent()
{
	m_Player.phShip->SetParent(m_ParentLayer, 0);
	for (auto& bullet : m_PlayerBullets)
		bullet.phBullet->SetParent(m_ParentLayer, 0);
	for (auto& enemy : m_Enemies)
		enemy.phShip->SetParent(m_ParentLayer, 0);
	for (auto& bullet : m_EnemyBullets)
		bullet.phBullet->SetParent(m_ParentLayer, 0);
}

bool BattleManager::IsThereEnemies()
{
	//tmp for now, with spawner have to be updated
	return m_Enemies.size();
}

void BattleManager::Update(const float dt)
{
	if (!m_instance)
		return;

	updatePlayer(dt);
	updateEnemies(dt);
	updatePlayerBullets(dt);
	updateEnemyBullets(dt);
	checkForHitEnemy();
	checkForHitPlayer();

}

void BattleManager::updatePlayer(const float dt)
{
	//physical part
	m_Player.phShip->SetDirection(m_PlayerDirCB());
	m_Player.phShip->Update();

	//logical part
	m_Player.lShip->Update(dt);
	m_DisplayPlayerLifeCB(m_Player.lShip->GetLifeInPer());
	m_DisplayPlayerShieldCB(m_Player.lShip->GetShieldInPer());

	//input callbacks
	if ((m_PlayerButtonsCB() & 1) == 1)
		fireBullet(true, &m_Player);
}

void BattleManager::updateEnemies(const float dt)
{
	for (auto& enemy : m_Enemies)
	{
		AIMove move = enemy.ai(m_Player.phShip, enemy.phShip);
		enemy.phShip->SetDirection(move.newDir);
		enemy.phShip->SetVelocity(move.newVelocity);
		if (move.fire)
			fireBullet(false, &enemy);
		enemy.phShip->Update();
		enemy.lShip->Update(dt);
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

void BattleManager::updateBullets(std::vector<Bullet>& bulletArray, const float dt)
{
	for (auto it = bulletArray.begin(); it != bulletArray.end();)
	{
		if (!it->phBullet->Update())
		{
			delete it->phBullet;
			it = bulletArray.erase(it);
		}
		else
			++it;
	}
}

void BattleManager::checkForHitPlayer()
{
	for (auto it = m_EnemyBullets.begin(); it != m_EnemyBullets.end();)
	{
		if (it->phBullet->Collision(*(m_Player.phShip)))
		{
			//apply dmg
			m_Player.lShip->OnHit(it->lWeapon);

			//delete the bullet
			delete it->phBullet;
			it = m_EnemyBullets.erase(it);
		}
		else
			it++;
	}
}

void BattleManager::checkForHitEnemy()
{
	bool collision;
	for (auto itBullet = m_PlayerBullets.begin(); itBullet != m_PlayerBullets.end();)
	{
		collision = false;
		for (auto itEnemy = m_Enemies.begin(); itEnemy != m_Enemies.end();)
		{
			if (itBullet->phBullet->Collision(*(itEnemy->phShip)))
			{
				collision = true;
				//apply dmg
				itEnemy->lShip->OnHit(itBullet->lWeapon);

				//delete the bullet
				delete itBullet->phBullet;
				itBullet = m_PlayerBullets.erase(itBullet);

				//if enemy dies explosion and delete it
				if (!itEnemy->lShip->IsAlive()) {
					//TODO: lShip and weapon leaks
					auto forDel = *itEnemy;
					itEnemy = m_Enemies.erase(itEnemy);
					startExplosion(forDel.phShip);
				}
				break;
			}
			else
				++itEnemy;
		}
		if(!collision) 
			++itBullet;
	}
}

void BattleManager::fireBullet(bool isPlayerBullet, Player * shooter)
{
	if (!shooter->lShip->GetWeapon()->CanShoot())
		return;

	shooter->lShip->GetWeapon()->Shoot();
	const char * spriteNames[2] = {"EnemyBullet.png", "bullet.png"};
	BulletBase * bullet = new BulletBase(shooter->phShip->GetPosition(), shooter->phShip->GetDirection().getNormalized(), 25.f, spriteNames[isPlayerBullet]);
	bullet->SetVelocity(isPlayerBullet ? 25.f : 10.f);
	auto& bulletArray = isPlayerBullet ? m_PlayerBullets : m_EnemyBullets;
	bulletArray.push_back(Bullet{ bullet, shooter->lShip->GetWeapon() });
	bulletArray.back().phBullet->SetParent(m_ParentLayer, 0);
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
