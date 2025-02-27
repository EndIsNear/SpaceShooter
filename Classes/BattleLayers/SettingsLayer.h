#ifndef __SETTINGS_LAYER_H__
#define __SETTINGS_LAYER_H__

#include "cocos2d.h"

#include "PopupMenu.h"

class SettingsLayer : public PopupBase
{
public:
	virtual bool init() override;

	void onResume();
	void onRestart();
	void onQuit();
	CREATE_FUNC(SettingsLayer);
};

#endif //__SETTINGS_LAYER_H__