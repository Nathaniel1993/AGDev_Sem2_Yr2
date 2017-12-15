#pragma once
#include "WeaponInfo.h"
class CPlayerInfo;

class CSelector :
	public CWeaponInfo
{
public:
	CSelector();
	virtual ~CSelector();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};