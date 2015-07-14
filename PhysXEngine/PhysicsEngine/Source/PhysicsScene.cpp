#include "PhysicsScene.h"


PhysicsScene::PhysicsScene()
{

}

PhysicsScene::PhysicsScene(glm::vec2 _gravity)
{
	m_gravity = _gravity;
}

PhysicsScene::PhysicsScene(glm::vec2 _gravity, float _timeStep)
{
	m_gravity = _gravity;
	m_timeStep = _timeStep;
}

PhysicsScene::~PhysicsScene()
{
	m_actors.clear();
}

void PhysicsScene::AddActor(PhysicsObject* _actor)
{
	m_actors.push_back(_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* _actor)
{
	m_actors.erase(m_actors.begin(), m_actors.end());
}

void PhysicsScene::Update()
{
	for (auto i = m_actors.begin(); i < m_actors.end(); i++)
	{
		m_actors.at[i]->Update(m_gravity, m_timeStep);
	}
}

void PhysicsScene::DebugScene()
{

}

void PhysicsScene::RenderGizmos()
{
	for (auto i = m_actors.begin(); i < m_actors.end(); i++)
	{
		m_actors.at[i]->MakeGizmos();
	}
}