#ifndef __END_GAME_STATS_H__
#define __END_GAME_STATS_H__

#include "cocos2d.h"

#include "PopupMenu.h"

class EndGameStats : public PopupBase
{
	cocos2d::Label * m_Result;
	cocos2d::Label * m_Time;
public:
	virtual bool init() override;
	void SetStats(std::string stats);
	void SetTime(const float time);

	void onContinue();
	void onRestart();
	CREATE_FUNC(EndGameStats);
};

#endif //__ENDGAME_STATS_H__