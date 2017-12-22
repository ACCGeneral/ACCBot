#pragma once

#include "buildingcounts.h"
#include "UtilityFarm.h"

using namespace BWAPI;
	
class buildingscore : public UtilityFarm
{
private:
	
	bool gascooldown; 
	bool expandcooldown;
	float lastsecondgas;
	float lastsecondexpand;
	bool gasbool;
	bool expandbool;


	float supplyscore;
	float gasscore;
	float barrecksscore;
	float enginscore;
	float turretscore;
	float acadamyscore;
	float factoryscore;
	float starport;
	float armoryscore;
	float sciencefasility;
	float expandscore;

public:

	void update();
	buildingscore(productionmanager *p);
	buildinfo scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder);
	void resetfromthreat(BWAPI::UnitType type);

	void setcooldown(BWAPI::UnitType type);
	void setcooldownbool(BWAPI::UnitType type);


};