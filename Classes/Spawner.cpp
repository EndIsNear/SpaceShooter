#include "Spawner.h"

bool Spawner::getElementForTime(const float time, SpawnElement& res)
{
	if (m_SpawnElements[m_crnIdx].time < time && m_crnIdx > m_SpawnElements.size())
	{
		res = m_SpawnElements[m_crnIdx++];
		return true;
	}
	return false;
}

