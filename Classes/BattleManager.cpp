#include "BattleManager.h"
#include <random>

USING_NS_CC;

BattleManager * BattleManager::m_instance = nullptr;

///////////////////////////
//test
std::mt19937 rng;
AIMove shit(ShipBase * enemy, std::vector<BulletBase*> b, ShipBase * self)
{
	std::uniform_real_distribution<> v(0, 5);
	AIMove res;
	res.newVelocity = v(rng);
	res.newDir = (enemy->GetPosition() - self->GetPosition()).getNormalized();
	res.fire = v(rng) > 4.8f;
	return res;
}
///////////////////////////

void BattleManager::initialize()
{
	///////////////////////////
	//test
	m_Player.phShip = new ShipBase(Vec2(1920, 1080), Vec2(1, 0), 10.f, "fighter.png");
	m_Player.lShip = new LogicalShip(1000, 500, 33, 0, new LogicalWeapon(200.f, 0.5f));
	m_Player.phShip->Update();

	rng.seed(std::random_device()());
	ShipBase * enemy = new ShipBase(Vec2(1920, 540), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	LogicalShip * lEnemy = new LogicalShip(1000, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(960, 1080), Vec2(0, 0), 10.f, "enemies/Enemy4.png");
	enemy->Update();
	lEnemy = new LogicalShip(1000, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(960, 1620), Vec2(0, 0), 10.f, "enemies/Enemy3.png");
	enemy->Update();
	lEnemy = new LogicalShip(1000, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(960, 540), Vec2(0, 0), 10.f, "enemies/Enemy6.png");
	enemy->Update();
	lEnemy = new LogicalShip(1000, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(2780, 1620), Vec2(0, 0), 10.f, "enemies/Enemy1.png");
	enemy->Update();
	lEnemy = new LogicalShip(1000, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(1920, 1620), Vec2(0, 0), 10.f, "enemies/Enemy2.png");
	enemy->Update();
	lEnemy = new LogicalShip(1000, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));
	enemy = new ShipBase(Vec2(2780, 1080), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	lEnemy = new LogicalShip(1000, 100, 33, 0, new LogicalWeapon(100.f, 1.f));
	m_Enemies.push_back(Enemy(shit, Player{ lEnemy, enemy }));

	///////////////////////////
}

void BattleManager::free()
{
	//free player
	delete m_Player.phShip;
	delete m_Player.lShip;
	for (auto& bullet : m_PlayerBullets)
		delete bullet;
	m_PlayerBullets.resize(0);

	//free enemy
	for (auto& enemy : m_Enemies)
	{
		delete enemy.phShip;
		delete enemy.lShip;
	}
	m_Enemies.resize(0);
	for (auto& bullet : m_EnemyBullets)
		delete bullet;
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
		bullet->SetParent(m_ParentLayer, 0);
	for (auto& enemy : m_Enemies)
		enemy.phShip->SetParent(m_ParentLayer, 0);

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
		AIMove move = enemy.ai(m_Player.phShip, m_PlayerBullets, enemy.phShip);
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

void BattleManager::updateBullets(std::vector<BulletBase*>& bulletArray, const float dt)
{
	for (auto it = bulletArray.begin(); it != bulletArray.end();)
	{
		if (!(*it)->Update())
		{
			delete *it;
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
		if ((*it)->Collision(*(m_Player.phShip)))
		{
			//delete the bullet
			delete *it;
			it = m_EnemyBullets.erase(it);
		}
		else
			it++;
	}
}

void BattleManager::checkForHitEnemy()
{
	for (auto& bullet : m_PlayerBullets)
	{
		for (auto it = m_Enemies.begin(); it != m_Enemies.end();)
		{
			if (bullet->Collision(*(it->phShip)))
			{
				auto forDel = *it;
				it = m_Enemies.erase(it);
				startExplosion(forDel.phShip);
			}
			else
				++it;
		}
	}
}

void BattleManager::fireBullet(bool isPlayerBullet, Player * shooter)
{
	if (!shooter->lShip->GetWeapon()->CanShoot())
		return;

	shooter->lShip->GetWeapon()->Shoot();
	const char * spriteNames[2] = {"EnemyBullet.png", "bullet.png"};
	BulletBase * bullet = new BulletBase(shooter->phShip->GetPosition(), shooter->phShip->GetDirection().getNormalized(), 25.f, spriteNames[isPlayerBullet]);
	bullet->SetVelocity(25.f);
	auto& bulletArray = isPlayerBullet ? m_PlayerBullets : m_EnemyBullets;
	bulletArray.push_back(bullet);
	bulletArray.back()->SetParent(m_ParentLayer, 0);
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
