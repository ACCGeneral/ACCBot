#pragma once

#include "buildingcounts.h"
#include "Counterlist.h"
#include "UtilityFarm.h"

using namespace BWAPI;

class unitscoring : public UtilityFarm
{
private:

	float workerscore;
	float marinescore;
	float medicscore;
	float firebatscore;
	float ghostscore;
	float tankscore;
	float Vulturescore;
	float goliathscore;
	float Wraithscore;
	float SVscore;
	float Bcscore;
	float valkscore;

public:

	bool affordcheck(BWAPI::UnitType checktype);

	unitscoring(productionmanager *p);
	buildinfo scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder);
	void update()
	{

	}
	void setcooldown(BWAPI::UnitType type)
	{

	}
	void setcooldownbool(BWAPI::UnitType type)
	{

	}

	void resetfromthreat(BWAPI::UnitType type);

};
