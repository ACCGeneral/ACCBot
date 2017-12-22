#pragma once

#include "BWAPI.h"
#include "BWTA.h"
#include <vector>
#include "unitmanager.h"
#include "utilityfunction.h"
#include "usefulFunctions.h"

using namespace BWAPI;

class SegieManager
{
private:

	std::set<BWAPI::Unit> seigeTank;
	unitmanager *man;
	state mystate;

	float deplymodeset;
	int basepoint;

public:

	SegieManager(unitmanager *unitman);

	void addunit(BWAPI::Unit myunit)
	{
		seigeTank.insert(myunit);
	}
	void deleteunit(BWAPI::Unit myunit)
	{
		seigeTank.erase(myunit);
	}


	void update(state currentstate);

	void retreatcall();

	void scoredeply();

	float getclosestenemydis();

};