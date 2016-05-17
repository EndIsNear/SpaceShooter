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
		tmp.phShip = new EnemyShip(levelInfo.m_SpawnPoints[entry.spwnPntID].first, Vec2(1.f, 0.f), entry.shipSpeed,
			spNames[entry.sprtNameID]);
		tmp.lShip = new LogicalShip(*lShips[entry.lShipID]);
		tmp.lShip->SetLogicalWeapon(new LogicalWeapon(*lWeps[entry.lWeaponID]));
		tmp.ai = GetAIbyID(*tmp.phShip, *(tmp.lShip->GetWeapon()), entry.aiID);
		tmp.phShip->Update(0.f);
		m_SpawnElements.emplace_back(entry.timeToSpawn, tmp);
	}
}

Spawner::~Spawner()
{
	for (auto& entry : m_SpawnElements)
	{
		delete entry.second.lShip;
		delete entry.second.phShip;
		delete entry.second.ai;
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

