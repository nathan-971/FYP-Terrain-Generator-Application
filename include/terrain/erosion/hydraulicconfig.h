#ifndef _HYDRAULIC_CONFIG_H_
#define _HYDRAULIC_CONFIG_H_

struct HydraulicConfig
{
    int dropLifetime = 20;
    int numberOfDrops = 70000;

    float inertia = 0.3f;
    float capacityFactor = 1.5f;
    float minCapacity = 0.01f;

    float erodeSpeed = 0.2f;
    float depositSpeed = 0.3f;
    float evaporation = 0.005f;

    float minHeight = 0.1f;
    float maxErodePerStep = 0.5f;
    float maxDepositPerStep = 0.5f;
};

#endif