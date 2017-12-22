#pragma once

#include "BWAPI.h"
#include "BWTA.h"
#include <vector>
#include "unitmanager.h"
#include "utilityfunction.h"
#include "usefulFunctions.h"

using namespace BWAPI;

class MedicManger	
{
private:

	std::set<BWAPI::Unit> medics;
	std::set<BWAPI::Unit> healable;
	unitmanager *man;
	state mystate;

	int healcheck;
	int lasthealcheck;
	int basepoint;

public:

	MedicManger(unitmanager *unitman);

	void update(state currentstate);

	void addunit(BWAPI::Unit myunit)
	{
		medics.insert(myunit);
	}
	void deleteunit(BWAPI::Unit myunit)
	{
		medics.erase(myunit);
	}


	void addhealable(BWAPI::Unit myunit)
	{
		healable.insert(myunit);
	}
	void deletehealable(BWAPI::Unit myunit)
	{
		healable.erase(myunit);
	}

	float scoreunit(BWAPI::Unit wantedunit, float currentscore, BWAPI::Unit medic);

	BWAPI::Position getclosestnonmedic(BWAPI::Unit me);


	void retreatcall();
};