#pragma once
#include "RagdollData.h"

class Ragdoll
{
public:

	RagdollNode* ragdollData[] =
	{
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NO_PARENT, 1, 3, 1, 1, "lowerSpine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), LOWER_SPINE, 1, 1, -1, 1, "leftPelvis"),
		new RagdollNode(PxQuat(0, Z_AXIS), LOWER_SPINE, 1, 1, -1, 1, "rightPelvis"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_PELVIS, 5, 2, -1, 1, "leftUpperLeg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_PELVIS, 5, 2, -1, 1, "rightUpperLeg"),
		new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS), LEFT_UPPER_LEG, 5, 1.75f, -1, 1, "leftLowerLeg"),
		new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS), RIGHT_UPPER_LEG, 5, 1.75f, -1, 1, "rightLowerLeg"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), LOWER_SPINE, 1, 3, 1, -1, "upperSpine"),
		new RagdollNode(PxQuat(PxPi, Z_AXIS), UPPER_SPINE, 1, 1.5f, 1, 1, "leftClavicle"),
		new RagdollNode(PxQuat(0, Z_AXIS), UPPER_SPINE, 1, 1.5f, 1, 1, "rightClavicle"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), UPPER_SPINE, 1, 1, 1, -1, "neck"),
		new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NECK, 1, 3, 1, -1, "head"),
		new RagdollNode(PxQuat(PxPi - 0.3f, Z_AXIS), LEFT_CLAVICLE, 3, 1.5f, -1, 1, "leftUpperArm"),
		new RagdollNode(PxQuat(0.3f, Z_AXIS), RIGHT_CLAVICLE, 3, 1.5f, -1, 1, "rightUpperArm"),
		new RagdollNode(PxQuat(PxPi - 0.3f, Z_AXIS), LEFT_UPPER_ARM, 3, 1, -1, 1, "leftLowerArm"),
		new RagdollNode(PxQuat(0.3f, Z_AXIS), RIGHT_UPPER_ARM, 3, 1, -1, 1, "rightLowerArm"),
		NULL
	};

private:
};