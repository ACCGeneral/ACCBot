#pragma once

#include "UtilityFarm.h"
#include "buildingcounts.h"

class AddonScoring : public UtilityFarm
{
private:

	float physicslabscore;
	float covertops;
	float machineshop;
	float Ctower;
	float comstower;

public:

	AddonScoring(productionmanager *p);
	buildinfo scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder);

	void setcooldown(BWAPI::UnitType type)
	{

	}
	void setcooldownbool(BWAPI::UnitType type)
	{

	}
	void update()
	{


	}

	void resetfromthreat(BWAPI::UnitType type);

};