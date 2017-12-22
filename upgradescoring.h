#pragma once

#include "UtilityFarm.h"
#include "buildingcounts.h"
#include "UnitCount.h"

using namespace BWAPI;

class upgradescoring : public UtilityFarm
{
private:

	float marinerangescore;
	float seigescore;
	float valturescore;
	float manattackscore;
	float manarmorscore;
	float vehicalattackscore;
	float vehicalarmorscore;
	float shipattackscore;
	float shiparmorscore;

public:

	upgradescoring(productionmanager *p);
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


	float getnumberscale(BWAPI::UpgradeType myupgrade, int defalutaddon);

	bool upgradecheck(BWAPI::UpgradeType myupgrade, std::vector<buildinfo> buildorder);

	bool checkbuildandgame(std::vector<buildinfo> buildorder, BWAPI::UpgradeType myupgrade, BWAPI::UnitType mybuilder);

};