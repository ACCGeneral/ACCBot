#pragma once

#include "BWAPI.h"
#include "BWTA.h"
#include <vector>
#include "unitmanager.h"
#include "utilityfunction.h"
#include "usefulFunctions.h"

using namespace BWAPI;

class detectorManagaer
{
private:

	std::set<BWAPI::Unit> detectors;
	unitmanager *man;
	state mystate;

public:

	detectorManagaer(unitmanager *unitman);

	void addunit(BWAPI::Unit myunit)
	{
		detectors.insert(myunit);
	}
	void deleteunit(BWAPI::Unit myunit)
	{
		detectors.erase(myunit);
	}


	void update(state currentstate);

	void retreatcall();

	void getclosestclock(BWAPI::Unit mydetector);

};