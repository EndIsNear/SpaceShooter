#include "Shaders.h"

USING_NS_CC;

GLProgram * GetOnHitShader()
{
	//TODO: cache this on first call!
	GLProgram * p = new GLProgram();
	p->initWithFilenames("Shaders/OnHit.vsh", "Shaders/OnHit.fsh");
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	p->link();
	p->updateUniforms();

	return p;
}

void UpdateOnHitShader(GLProgram *p, const float time)
{
	GLProgramState* state = GLProgramState::getOrCreateWithGLProgram(p);
	GLuint ctime = glGetUniformLocation(p->getProgram(), "u_ctime");
	state->setUniformFloat(ctime, fmod(time, 1.f));
	GLuint gtime = glGetUniformLocation(p->getProgram(), "u_gtime");
	state->setUniformFloat(gtime, 1.f);
	GLuint color = glGetUniformLocation(p->getProgram(), "u_color");
	state->setUniformVec4(color, cocos2d::Vec4(0.1f, 0.2f, 0.8f, 1.f));
}

cocos2d::GLProgram * GetCooldownShader()
{
	//TODO: cache this on first call!
	GLProgram * p = new GLProgram();
	p->initWithFilenames("Shaders/Cooldown.vsh", "Shaders/Cooldown.fsh");
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	p->link();
	p->updateUniforms();
	return p;
}

void UpdateCooldownShader(GLProgram *p, const float percent)
{

	GLProgramState* state = GLProgramState::getOrCreateWithGLProgram(p);

	GLuint u_percent = glGetUniformLocation(p->getProgram(), "u_percent");
	state->setUniformFloat(u_percent, percent);
}