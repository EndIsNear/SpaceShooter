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

	std::unique_ptr<SkillInterface> skill1(new DualBarrelAttack(50.f, 0.5f, 800.f, 30.f, "bullet.png"));
	std::unique_ptr<SkillInterface> skill2(new GranadeAttack(500.f, 5.f, 800.f, 0.5f, "EnemyBullet.png", *(skill1.get())));
	std::unique_ptr<SkillInterface> skill3(new IncAttackSpeedSkill(10.f, 2.f, 5.f));
	std::unique_ptr<SkillInterface> skill4(new DoTAttack(500.f, 2.f, 800.f, "bullet2.png", 10.f));
	auto weapon = new LogicalWeapon(skill1, skill2, skill3, skill4);

	m_Allies.lShips.push_back(new LogicalShip(1000, 500, 33, 0, weapon));

	m_Allies.ais.push_back(nullptr);
	m_Allies.phShips[m_PlayerIndex]->Update(0.f);
	auto  p = GetOnHitShader();
	m_state = GLProgramState::create(p);
	//m_Allies.phShips[m_PlayerIndex]->GetSprite()->setGLProgram(p);
	m_Allies.phShips[m_PlayerIndex]->GetSprite()->setGLProgramState(m_state);

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
	m_PlayerBullets.skills.resize(0);

	////free enemy
	m_Enemies.phShips.resize(0);
	m_Enemies.lShips.resize(0);
	m_Enemies.ais.resize(0);


	for (size_t i = 0; i < m_EnemyBullets.Size(); ++i)
	{
		delete m_EnemyBullets.bullets[i];
	}
	m_EnemyBullets.bullets.resize(0);
	m_EnemyBullets.skills.resize(0);

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

bool BattleManager::GetPlayerCooldowns(float(&cdns)[4])
{
	bool res = true;
	for (size_t i = 0; i < 4; ++i)
		cdns[i] = m_Allies.lShips[0]->GetWeapon()->GetPrcCooldownAt(i);
	return res;
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
	UpdateOnHitShader(m_state, m_ElapsedTime);
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
		bool doOnExplosion = false;
		const bool shouldErase = !bulletArray.bullets[i]->Update(dt, &doOnExplosion);
		if (doOnExplosion)
		{
			auto res = bulletArray.skills[i]->OnExplosion(bulletArray.bullets[i]->GetPosition(), bulletArray.bullets[i]->GetDirection());
			if (res.m_Type == SkillResult::ResultType::Bullet && res.m_Bullets[0])
			{
				applySkillBulletsResult(true, res);
			}
		}

		if (shouldErase)
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
				m_Allies.lShips[j]->OnHit(m_EnemyBullets.skills[i]);

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
				m_Enemies.lShips[j]->OnHit(m_PlayerBullets.skills[i]);
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
	if (res.m_Type == SkillResult::ResultType::Bullet && res.m_Bullets[0])
	{
		applySkillBulletsResult(isPlayerBullet, res);
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

void BattleManager::applySkillBulletsResult(bool playerBullets, SkillResult& res)
{
	auto& bulletArray = playerBullets ? m_PlayerBullets : m_EnemyBullets;
	bulletArray.bullets.insert(bulletArray.bullets.end(), res.m_Bullets.begin(), res.m_Bullets.end());
	for (size_t i = 0; i < res.m_Bullets.size(); ++i)
	{
		bulletArray.bullets[bulletArray.skills.size()]->SetParent(m_ParentLayer, 0);
		bulletArray.skills.push_back(res.m_Source);
	}
}
