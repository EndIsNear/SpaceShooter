#include "Shaders.h"

USING_NS_CC;

static GLProgram* getOrCreateShader(const std::string& name, const std::string& vert, const std::string& frag)
{
	auto cache = GLProgramCache::getInstance();
	auto prog = cache->getGLProgram(name);
	if (prog == nullptr)
	{
		prog = GLProgram::createWithFilenames(vert, frag);
		cache->addGLProgram(prog, name);
	}
	return prog;
}


GLProgram * GetOnHitShader()
{
	GLProgram * p = getOrCreateShader("OnHit", "Shaders/OnHit.vert", "Shaders/OnHit.frag");
	return p;
}

void UpdateOnHitShader(GLProgramState * state, const float time)
{
	//GLProgramState* state = GLProgramState::getOrCreateWithGLProgram(p);
	state->setUniformFloat("u_ctime", fmod(time, 1.f));
	state->setUniformFloat("u_gtime", 1.f);
	state->setUniformVec4("u_color", cocos2d::Vec4(0.1f, 0.2f, 0.8f, 1.f));
}

cocos2d::GLProgram * GetCooldownShader()
{
	auto prog = getOrCreateShader("CooldownShader", "Shaders/Cooldown.vert", "Shaders/Cooldown.frag");
	return prog;
}

void UpdateCooldownShader(GLProgramState * state, const float percent)
{
	state->setUniformFloat("u_percent", percent);
}

