#pragma once
#include "RagdollData.h"

const int NUM_BONES = 16;

class Ragdoll
{
public:

	Ragdoll();

	PxArticulation* MakeRagdoll(PxPhysics* _physics, RagdollNode** _nodeArray, PxTransform _worldPos, float _scaleFactor, PxMaterial* _ragdollMaterial);

	RagdollNode** m_ragdollData;

private:
};