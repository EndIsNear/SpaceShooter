#include "Spawner.h"

USING_NS_CC;

void Spawner::Initialize()
{
	SpawnElement tmp;
	tmp.phShip = new ShipBase(Vec2(1920, 540), Vec2(1, 0), 400.f, "enemies/Enemy5.png");
	tmp.lShip = new LogicalShip(1000, 700, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	std::mt19937 rng;
	AIBaseStruct basestruct{ cocos2d::Size(0.,0.),rng,*tmp.phShip, *(tmp.lShip->GetWeapon()) };

	tmp.ai = new StayAwayEndShoot(basestruct);

	m_SpawnElements.emplace_back(2.f, tmp);
	//tmp.phShip->Update(0.f);
	//dynamic_cast<AIPointToPoint*>(tmp.ai)->AddPoint(cocos2d::Vec2(200, 200));
	//dynamic_cast<AIPointToPoint*>(tmp.ai)->AddPoint(cocos2d::Vec2(2200, 1200));

	//m_SpawnElements.emplace_back(2.f, tmp);

	//tmp.phShip = new ShipBase(Vec2(960, 1080), Vec2(1, 0), 400.f, "enemies/Enemy4.png");
	//tmp.phShip->Update(0.f);
	//tmp.lShip = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	//tmp.ai = new AICoward(*tmp.phShip, *(tmp.lShip->GetWeapon()), 250, 400);
	//m_SpawnElements.push_back(std::pair<float, SpawnElement>(3.f, tmp));
	/*
	tmp.phShip = new ShipBase(Vec2(960, 1620), Vec2(1, 0), 400.f, "enemies/Enemy3.png");
	tmp.phShip->Update(0.f);
	tmp.lShip = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	tmp.ai = new AICoward(*tmp.phShip, *(tmp.lShip->GetWeapon()), 250, 400);
	m_SpawnElements.push_back(std::pair<float, SpawnElement>(4.f, tmp));

	tmp.phShip = new ShipBase(Vec2(960, 540), Vec2(1, 0), 400.f, "enemies/Enemy6.png");
	tmp.phShip->Update(0.f);
	tmp.lShip = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	tmp.ai = new AICoward(*tmp.phShip, *(tmp.lShip->GetWeapon()), 250, 400);
	m_SpawnElements.push_back(std::pair<float, SpawnElement>(5.f, tmp));

	tmp.phShip = new ShipBase(Vec2(2780, 1620), Vec2(1, 0), 400.f, "enemies/Enemy1.png");
	tmp.phShip->Update(0.f);
	tmp.lShip = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	tmp.ai = new AIBase(*tmp.phShip, *(tmp.lShip->GetWeapon()));
	m_SpawnElements.push_back(std::pair<float, SpawnElement>(6.f, tmp));

	tmp.phShip = new ShipBase(Vec2(1920, 1620), Vec2(1, 0), 400.f, "enemies/Enemy2.png");
	tmp.phShip->Update(0.f);
	tmp.lShip = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	tmp.ai = new AIBase(*tmp.phShip, *(tmp.lShip->GetWeapon()));
	m_SpawnElements.push_back(std::pair<float, SpawnElement>(7.f, tmp));

	tmp.phShip = new ShipBase(Vec2(2780, 1080), Vec2(1, 0), 400.f, "enemies/Enemy5.png");
	tmp.phShip->Update(0.f);
	tmp.lShip = new LogicalShip(300, 100, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	tmp.ai = new AIBase(*tmp.phShip, *(tmp.lShip->GetWeapon()));
	m_SpawnElements.push_back(std::pair<float, SpawnElement>(8.f, tmp));*/
}

bool Spawner::GetElementForTime(const float time, SpawnElement& res)
{
	if (m_crnIdx < m_SpawnElements.size() && m_SpawnElements[m_crnIdx].first < time)
	{
		res = m_SpawnElements[m_crnIdx++].second;
		return true;
	}
	return false;
}

