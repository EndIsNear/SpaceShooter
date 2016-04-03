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
		tmp.phShip = new ShipBase(levelInfo.m_SpawnPoints[entry.spwnPntID].first, Vec2(1.f, 0.f), 400.f, spNames[entry.sprtNameID]);

		tmp.lShip = new LogicalShip(*lShips[entry.lShipID]);
		tmp.lShip->SetLogicalWeapon(new LogicalWeapon(*lWeps[entry.lWeaponID]));
		//TODO: pick right AI
		tmp.ai = new AICoward(*tmp.phShip, *(tmp.lShip->GetWeapon()), 250, 400);
		tmp.phShip->Update(0.f);
		m_SpawnElements.emplace_back(entry.timeToSpawn, tmp);
	}
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

