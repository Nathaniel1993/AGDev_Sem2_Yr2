#include "StatesTank.h"
#include "../Waypoint/WaypointManager.h"

//===========================StateTankPatrol================================
StateTankPatrol::StateTankPatrol(const std::string &stateID, CTank* tank)
	: State(stateID),
	m_tank(tank)
{
}

StateTankPatrol::~StateTankPatrol()
{
}

void StateTankPatrol::Enter()
{
	m_speed = 30.f;
}

void StateTankPatrol::Update(double dt)
{
	//m_speed = m_tank->GetSpeed();
	Vector3 Vel = (m_tank->NextNode - m_tank->GetPosition()).Normalized() * m_speed * (float)dt;
	m_tank->SetPosition(m_tank->GetPosition() + Vel);

	for (std::vector<CTankPart*>::iterator it = m_tank->ChildList.begin(); 
		it != m_tank->ChildList.end(); ++it)
	{
		(*it)->SetPosition(m_tank->GetPosition() + (*it)->PartOffset);
	}

	float distSquared = (m_tank->NextNode - m_tank->GetPosition()).LengthSquared();
	if (distSquared < (3.f * 3.f))
	{
		m_tank->SetCurrWaypoint(m_tank->GetCurrWaypoint() + 1);
		if (m_tank->GetCurrWaypoint() >= CWaypointManager::GetInstance()->GetNumberOfWaypoints())
			m_tank->SetCurrWaypoint(0);
		
		m_tank->PrevNode = m_tank->NextNode;
		m_tank->NextNode = CWaypointManager::GetInstance()->GetWaypoint(m_tank->GetCurrWaypoint())->GetPosition();
	}
	
}

void StateTankPatrol::Exit()
{

}

//===========================StateTankChase================================
StateTankChase::StateTankChase(const std::string &stateID, CTank* tank)
	: State(stateID),
	m_tank(tank)
{
}

StateTankChase::~StateTankChase()
{
}

void StateTankChase::Enter()
{

}

void StateTankChase::Update(double dt)
{

}

void StateTankChase::Exit()
{

}

//===========================StateTankSearch================================
StateTankSearch::StateTankSearch(const std::string &stateID, CTank* tank)
	: State(stateID),
	m_tank(tank)
{
}

StateTankSearch::~StateTankSearch()
{
}

void StateTankSearch::Enter()
{

}

void StateTankSearch::Update(double dt)
{

}

void StateTankSearch::Exit()
{

}

//===========================StateTankReturn================================
StateTankReturn::StateTankReturn(const std::string &stateID, CTank* tank)
	: State(stateID),
	m_tank(tank)
{
}

StateTankReturn::~StateTankReturn()
{
}

void StateTankReturn::Enter()
{

}

void StateTankReturn::Update(double dt)
{

}

void StateTankReturn::Exit()
{

}

//===========================StateTankAttack================================
StateTankAttack::StateTankAttack(const std::string &stateID, CTank* tank)
	: State(stateID),
	m_tank(tank)
{
}

StateTankAttack::~StateTankAttack()
{
}

void StateTankAttack::Enter()
{

}

void StateTankAttack::Update(double dt)
{

}

void StateTankAttack::Exit()
{

}

//===========================StateTankRetreat================================
StateTankRetreat::StateTankRetreat(const std::string &stateID, CTank* tank)
	: State(stateID),
	m_tank(tank)
{
}

StateTankRetreat::~StateTankRetreat()
{
}

void StateTankRetreat::Enter()
{

}

void StateTankRetreat::Update(double dt)
{

}

void StateTankRetreat::Exit()
{

}