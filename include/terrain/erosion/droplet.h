#ifndef _DROPLET_H_
#define _DROPLET_H_

struct Droplet
{
	float posX = 0.0f;
	float posZ = 0.0f;

	float dirX = 0.0f;
	float dirZ = 0.0f;

	float speed = 1.0f;
	float water = 1.0f;
	float sediment = 0.0f;
};

#endif