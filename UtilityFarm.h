#pragma once

#include "BWAPI.h"
#include "utilityfunction.h"
#include "usefulFunctions.h"
#include "buildinfo.h"
#include "Worldinformation.h"
#include "BWTA.h"

class productionmanager;


class UtilityFarm
{
private:

protected:

	productionmanager *pro;

	float newtechbonus;

public:

	UtilityFarm(productionmanager *p);
	virtual buildinfo scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder) = 0;
	virtual void update() = 0;
	virtual void setcooldown(BWAPI::UnitType type) = 0;
	virtual void setcooldownbool(BWAPI::UnitType type) = 0;
	virtual void resetfromthreat(BWAPI::UnitType type) = 0;

	int getcountbuilding(BWAPI::UnitType type, std::vector<buildinfo> buildorder);
	int getcountUnit(BWAPI::UnitType type, std::vector<buildinfo> buildorder);

};

