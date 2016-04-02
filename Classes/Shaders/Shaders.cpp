#include "Shaders.h"

USING_NS_CC;

GLProgram * GetOnHitShader()
{
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
	state->setUniformVec4(color, cocos2d::Vec4(0.5f, 0.f, 0.f, 1.f));
}
