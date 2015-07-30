#include "Ragdoll.h"


Ragdoll::Ragdoll()
{
	m_ragdollData = new RagdollNode*[17]
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
	};;
}

PxArticulation* Ragdoll::MakeRagdoll(PxPhysics* _physics, RagdollNode** _nodeArray, PxTransform _worldPos, float _scaleFactor, PxMaterial* _ragdollMaterial)
{
	PxArticulation* articulation = _physics->createArticulation();
	RagdollNode** currentNode = _nodeArray;

	while (*currentNode != NULL)
	{
		RagdollNode* currentNodePtr = *currentNode;
		RagdollNode* parentNode = nullptr;

		float radius = currentNodePtr->radius * _scaleFactor;
		float halfLength = currentNodePtr->halfLength * _scaleFactor;
		float childHalfLength = radius + halfLength;
		float parentHalfLength = 0;

		PxArticulationLink* parentLinkPtr = NULL;
		currentNodePtr->scaledGlobalPos = _worldPos.p;

		if (currentNodePtr->parentID != -1)
		{
			parentNode = *(_nodeArray + currentNodePtr->parentID);

			parentLinkPtr = parentNode->linkPtr;
			parentHalfLength = (parentNode->radius + parentNode->halfLength) * _scaleFactor;

			//calculate the local position of the node
			PxVec3 currentRelative = currentNodePtr->childLinkPos * currentNodePtr->globalRotation.rotate(PxVec3(childHalfLength, 0, 0));
			PxVec3 parentRelative = -currentNodePtr->parentPos * parentNode->globalRotation.rotate(PxVec3(parentHalfLength, 0, 0));

			currentNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos - (parentRelative + currentRelative);
		}

		PxTransform linkTransform = PxTransform(currentNodePtr->scaledGlobalPos, currentNodePtr->globalRotation);
		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);
		currentNodePtr->linkPtr = link;

		float jointSpace = 0.01f; //gap between joints
		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + 0.01f;

		//adding a capusle collider to the link
		PxCapsuleGeometry capsule(radius, capsuleHalfLength);
		link->createShape(capsule, *_ragdollMaterial);
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f);

		if (currentNodePtr->parentID != -1)
		{
			PxArticulationJoint* joint = link->getInboundJoint();
			PxQuat frameRotation = parentNode->globalRotation.getConjugate() * currentNodePtr->globalRotation;
			PxTransform parentConstraintFrame = PxTransform(PxVec3(currentNodePtr->parentPos * parentHalfLength, 0, 0), frameRotation);
			PxTransform thisContraintFrame = PxTransform(PxVec3(currentNodePtr->childLinkPos * childHalfLength, 0, 0));

			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisContraintFrame);
			joint->setStiffness(20);
			joint->setDamping(20);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);
		}

		currentNode++;
	}

	return articulation;
}