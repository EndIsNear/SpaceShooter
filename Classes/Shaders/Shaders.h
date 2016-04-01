#ifndef __SHADERS_H__
#define __SHADERS_H__
#include "cocos2d.h"

cocos2d::GLProgram * GetOnHitShader();
void UpdateOnHitShader(cocos2d::GLProgram *p, const float time);



#endif //__SHADERS_H__