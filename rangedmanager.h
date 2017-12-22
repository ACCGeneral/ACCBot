#pragma once

#include "BWAPI.h"
#include <set>
#include "states.h"

using namespace BWAPI;


class unitmanager;
class rangedmanager
{
private:

	unitmanager *man;
	std::set<BWAPI::Unit> rangedunits;
	state mystate;

	int basepoint;

	BWAPI::Unit ClosestUnitAttack(BWAPI::Unit unit);
	
public:

	rangedmanager(unitmanager *myman)
	{
		man = myman;
		basepoint = 0;
	}

	void addunit(BWAPI::Unit myunit)
	{
		rangedunits.insert(myunit);
	}
	void deleteunit(BWAPI::Unit myunit)
	{
		rangedunits.erase(myunit);
	}

	void update(state currentstate);

	void retreatcall();

};