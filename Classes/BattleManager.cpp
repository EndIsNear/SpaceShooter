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
	res.fire = v(rng) > 4.95f;
	return res;
}
///////////////////////////

void BattleManager::initialize()
{
	m_PlayerShip = new ShipBase(Vec2(1920, 1080), Vec2(1, 0), 10.f, "fighter.png");
	m_PlayerShip->Update();
	m_PlayerAlive = 1;

	///////////////////////////
	//test
	rng.seed(std::random_device()());
	test = 100.f;
	ShipBase * enemy = new ShipBase(Vec2(1920, 540), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	m_Enemies.push_back(Enemy{ shit, enemy });
	enemy = new ShipBase(Vec2(960, 1080), Vec2(0, 0), 10.f, "enemies/Enemy4.png");
	enemy->Update();
	m_Enemies.push_back(Enemy{ shit, enemy });
	enemy = new ShipBase(Vec2(960, 1620), Vec2(0, 0), 10.f, "enemies/Enemy3.png");
	enemy->Update();
	m_Enemies.push_back(Enemy{ shit, enemy });
	enemy = new ShipBase(Vec2(960, 540), Vec2(0, 0), 10.f, "enemies/Enemy6.png");
	enemy->Update();
	m_Enemies.push_back(Enemy{ shit, enemy });
	enemy = new ShipBase(Vec2(2780, 1620), Vec2(0, 0), 10.f, "enemies/Enemy1.png");
	enemy->Update();
	m_Enemies.push_back(Enemy{ shit, enemy });
	///////////////////////////
}

void BattleManager::free()
{
	delete m_PlayerShip;
	for (auto& bullet : m_PlayerBullets)
		delete bullet;
	m_PlayerBullets.resize(0);
	for (auto& enemy : m_Enemies)
		delete enemy.ship;
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
	m_PlayerShip->SetParent(m_ParentLayer, 0);
	for (auto& bullet : m_PlayerBullets)
		bullet->SetParent(m_ParentLayer, 0);
	for (auto& enemy : m_Enemies)
		enemy.ship->SetParent(m_ParentLayer, 0);

}

void BattleManager::update()
{
	if (!m_instance)
		return;

	updatePlayer();
	updateEnemies();
	updatePlayerBullets();
	updateEnemyBullets();
	checkForHitEnemy();
	checkForHitPlayer();

	//test only
	m_DisplayPlayerLifeCB(test);
	test -= 0.05f;
}

void BattleManager::updatePlayer()
{
	m_PlayerShip->SetDirection(m_PlayerDirCB());
	m_PlayerShip->Update();
	if (m_PlayerButtonsCB() & 1 == 1)
		fireBullet(true, m_PlayerShip);
}

void BattleManager::updateEnemies()
{
	for (auto& enemy : m_Enemies)
	{
		AIMove move = enemy.ai(m_PlayerShip, m_PlayerBullets, enemy.ship);
		enemy.ship->SetDirection(move.newDir);
		enemy.ship->SetVelocity(move.newVelocity);
		if (move.fire)
			fireBullet(false, enemy.ship);
		enemy.ship->Update();
	}
}

void BattleManager::updatePlayerBullets()
{
	updateBullets(m_PlayerBullets);
}

void BattleManager::updateEnemyBullets()
{
	updateBullets(m_EnemyBullets);
}

void BattleManager::updateBullets(std::vector<BulletBase*>& bulletArray)
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
	for(auto& bullet : m_EnemyBullets)
	{
		if (bullet->Collision(*m_PlayerShip))
		{
			m_PlayerAlive = 0;
		}
	}
}

void BattleManager::checkForHitEnemy()
{
	for (auto& bullet : m_PlayerBullets)
	{
		for (auto it = m_Enemies.begin(); it != m_Enemies.end();)
		{
			if (bullet->Collision(*(it->ship)))
			{
				auto forDel = *it;
				it = m_Enemies.erase(it);
				startExplosion(forDel.ship);
			}
			else
				++it;
		}
	}
}

void BattleManager::fireBullet(bool isPlayerBullet, ShipBase * shooter)
{
	const char * spriteNames[2] = {"EnemyBullet.png", "bullet.png"};
	BulletBase * bullet = new BulletBase(shooter->GetPosition(), shooter->GetDirection().getNormalized(), 25.f, spriteNames[isPlayerBullet]);
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
