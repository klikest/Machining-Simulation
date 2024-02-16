#include "FrameBuffer.h"



FrameBuffer::FrameBuffer()
{

}

FrameBuffer::~FrameBuffer()
{

}


void FrameBuffer::Init()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}