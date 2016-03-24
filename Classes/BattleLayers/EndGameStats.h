#ifndef __END_GAME_STATS_H__
#define __END_GAME_STATS_H__

#include "cocos2d.h"

#include "PopupMenu.h"

class EndGameStats : public PopupBase
{
public:
	virtual bool init() override;

	void onContinue();
	CREATE_FUNC(EndGameStats);
};

#endif //__ENDGAME_STATS_H__