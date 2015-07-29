#pragma once 
#include <PxPhysicsAPI.h>

using namespace physx;

enum RagdollParts
{
	NO_PARENT = -1,
	LOWER_SPINE,
	LEFT_PELVIS,
	RIGHT_PELVIS,
	LEFT_UPPER_LEG,
	RIGHT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_LOWER_LEG,
	UPPER_SPINE,
	LEFT_CLAVICLE,
	RIGHT_CLAVICLE,
	NECK,
	HEAD,
	LEFT_UPPER_ARM,
	RIGHT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_LOWER_ARM
};

const PxVec3 X_AXIS = PxVec3(1, 0, 0);
const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
const PxVec3 Z_AXIS = PxVec3(0, 0, 1);

struct RagdollNode
{
	PxQuat globalRotation; //model space
	PxVec3 scaledGlobalPos; //world space

	int parentID;
	float halfLength;
	float radius;

	//relative position of link centre in parent to this node.
	float parentPos; //0 is the centre of hte node, -1 is left end of capsule and 1 is right end of capsule relative to X
	float childLinkPos; //relative position of link centre in child
	char* name;

	PxArticulationLink* linkPtr;

	RagdollNode(PxQuat _globalRotation, int _parentID, float _halfLength, float _radius, float _parentPos, float _childPos, char* _name)
	{
		globalRotation = _globalRotation;
		parentID = _parentID;
		halfLength = _halfLength;
		radius = _radius;
		parentPos = _parentPos;
		childLinkPos = _childPos;
		name = _name;
	}
};