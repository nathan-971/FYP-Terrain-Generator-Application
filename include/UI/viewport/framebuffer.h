#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

struct Framebuffer
{
	unsigned int fbo = 0;
	unsigned int color = 0;
	unsigned int depth = 0;
	int width = 0;
	int height = 0;
};

#endif