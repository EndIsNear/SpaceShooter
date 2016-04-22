#ifndef __SHADERS_H__
#define __SHADERS_H__
#include "cocos2d.h"

cocos2d::GLProgram * GetOnHitShader();
void UpdateOnHitShader(cocos2d::GLProgramState * state, const float time);

cocos2d::GLProgram * GetCooldownShader();
void UpdateCooldownShader(cocos2d::GLProgramState *p, const float time);


#endif //__SHADERS_H__