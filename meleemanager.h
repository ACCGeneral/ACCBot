#pragma once

#include "BWAPI.h"
#include <set>
#include "states.h"
using namespace BWAPI;

class unitmanager;
class meleemanager
{
private:

	std::set<BWAPI::Unit> meleeunits;
	unitmanager *man;
	state mystate;

	BWAPI::Unit ClosestUnitAttack(BWAPI::Unit unit);

	int basepoint;


public:

	meleemanager(unitmanager *unitman)
	{
		man = unitman;
		basepoint = 0;
	}

	void addunit(BWAPI::Unit myunit)
	{
		meleeunits.insert(myunit);
	}
	void deleteunit(BWAPI::Unit myunit)
	{
		meleeunits.erase(myunit);
	}

	void update(state currentstate);

	void retreatcall();

};