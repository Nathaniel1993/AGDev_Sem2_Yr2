#ifndef STATES_TANK_H
#define STATES_TANK_H
#include "State.h"
#include "../Tank/Tank.h"

//===========================StateTankPatrol================================
class StateTankPatrol : public State
{
	CTank* m_tank;
	float m_speed;
public:
	StateTankPatrol(const std::string &stateID, CTank* tank);
	virtual ~StateTankPatrol();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

//===========================StateTankChase================================
class StateTankChase : public State
{
	CTank* m_tank;
public:
	StateTankChase(const std::string &stateID, CTank* tank);
	virtual ~StateTankChase();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

//===========================StateTankSearch================================
class StateTankSearch : public State
{
	CTank* m_tank;
public:
	StateTankSearch(const std::string &stateID, CTank* tank);
	virtual ~StateTankSearch();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

//===========================StateTankReturn================================
class StateTankReturn : public State
{
	CTank* m_tank;
public:
	StateTankReturn(const std::string &stateID, CTank* tank);
	virtual ~StateTankReturn();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

//===========================StateTankAttack================================
class StateTankAttack : public State
{
	CTank* m_tank;
public:
	StateTankAttack(const std::string &stateID, CTank* tank);
	virtual ~StateTankAttack();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

//===========================StateTankRetreat================================
class StateTankRetreat : public State
{
	CTank* m_tank;
public:
	StateTankRetreat(const std::string &stateID, CTank* tank);
	virtual ~StateTankRetreat();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif