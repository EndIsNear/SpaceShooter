#include "BattleManager.h"

USING_NS_CC;

BattleManager * BattleManager::m_instance = nullptr;

void BattleManager::initialize()
{
	m_PlayerShip = new ShipBase(Vec2(1920, 1080), Vec2(1, 0), 10.f, "fighter.png");
	m_PlayerShip->Update();

	test = 100.f;
	ShipBase * enemy = new ShipBase(Vec2(1920, 540), Vec2(0, 0), 10.f, "enemies/Enemy5.png");
	enemy->Update();
	m_Enemies.push_back(enemy);
	enemy = new ShipBase(Vec2(960, 1080), Vec2(0, 0), 10.f, "enemies/Enemy4.png");
	enemy->Update();
	m_Enemies.push_back(enemy);
	enemy = new ShipBase(Vec2(960, 1620), Vec2(0, 0), 10.f, "enemies/Enemy3.png");
	enemy->Update();
	m_Enemies.push_back(enemy);
	enemy = new ShipBase(Vec2(960, 540), Vec2(0, 0), 10.f, "enemies/Enemy6.png");
	enemy->Update();
	m_Enemies.push_back(enemy);

}

void BattleManager::free()
{
	delete m_PlayerShip;
	for (auto& bullet : m_PlayerBullets)
		delete bullet;
	m_PlayerBullets.resize(0);
	for (auto& enemy : m_Enemies)
		delete enemy;
	m_Enemies.resize(0);
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
		enemy->SetParent(m_ParentLayer, 0);

}

void BattleManager::update()
{
	if (!m_instance)
		return;
	m_PlayerShip->SetDirection(m_PlayerDirCB());
	m_PlayerShip->Update();

	if (m_PlayerButtonsCB() & 1 == 1)
		fireBullet();
	
	for (auto it = m_PlayerBullets.begin(); it != m_PlayerBullets.end();)
	{
		if (!(*it)->Update())
		{
			delete *it;
			it = m_PlayerBullets.erase(it);
		}
		else
			++it;
	}

	checkForKilledEnemy();

	m_DisplayPlayerLifeCB(test);
	test -= 0.05f;
}

void BattleManager::fireBullet()
{
	BulletBase * bullet = new BulletBase(m_PlayerShip->GetPosition(), m_PlayerShip->GetDirection().getNormalized(), 25.f, "bullet.png");
	bullet->SetVelocity(25.f);
	m_PlayerBullets.push_back(bullet);
	m_PlayerBullets.back()->SetParent(m_ParentLayer, 0);
}

void BattleManager::checkForKilledEnemy()
{
	for (auto& bullet : m_PlayerBullets)
	{
		for (auto it = m_Enemies.begin(); it != m_Enemies.end();)
		{
			if (bullet->Collision(*(*it)))
			{
				auto forDel = *it;
				it = m_Enemies.erase(it);
				startExplosion(forDel);
				//delete forDel;
			}
			else
				++it;
		}
	}
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
