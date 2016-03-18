#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "cocos2d.h"

#define DESKTOP_PLATFORM (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) \
		|| (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) \
		|| (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) \

inline float GetRotation(const cocos2d::Vec2& l)
{
	return 90.f - CC_RADIANS_TO_DEGREES(l.getAngle());
}

#endif //__UTILITIES_H__