#include "Spawner.h"
#include "GameLogic\GameManager.h"

USING_NS_CC;

void Spawner::Initialize(size_t levelID)
{
	auto gm = GameManager::Instance();
	auto lShips = gm->GetLogicalShips();
	auto lWeps = gm->GetLogicalWeapons();
	auto spNames = gm->GetSpriteNames();
	auto levelInfo = gm->GetLevelInfo(levelID);

	SpawnElement tmp;
	for (auto& entry : levelInfo.m_SpawnEntries)
	{
		//TODO: remove hardcoded 400.f
		tmp.phShip = new ShipBase(levelInfo.m_SpawnPoints[entry.spwnPntID].first, Vec2(1.f, 0.f), entry.shipSpeed,
			spNames[entry.sprtNameID]);

		tmp.lShip = new LogicalShip(*lShips[entry.lShipID]);
		tmp.lShip->SetLogicalWeapon(new LogicalWeapon(*lWeps[entry.lWeaponID]));
		//TODO: pick right AI
		tmp.ai = GetAIbyID(*tmp.phShip, *(tmp.lShip->GetWeapon()), 0);
		tmp.phShip->Update(0.f);
		m_SpawnElements.emplace_back(entry.timeToSpawn, tmp);
	}

	//SpawnElement tmp;
	//tmp.phShip = new ShipBase(Vec2(1920, 540), Vec2(1, 0), 400.f, "enemies/Enemy5.png");
	//tmp.lShip = new LogicalShip(1000, 700, 33, 0, new LogicalWeapon(100.f, 1.f, 700.f));
	//std::mt19937 rng;
	//
	//tmp.ai = GetAIbyID(*tmp.phShip, *(tmp.lShip->GetWeapon()), 1);

	//m_SpawnElements.emplace_back(2.f, tmp);
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

