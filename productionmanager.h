#pragma once

#include "workermanager.h"
#include <BWAPI.h>
#include "buldingmanager.h"
#include "buildinfo.h"
#include "Worldinformation.h"
#include "influenceMapData.h"
#include "buildingplacer.h"
#include "buildingtimedata.h"
#include "expantionbuilder.h"
#include "UtilityFarm.h"
#include "buildingscoring.h"
#include "unitscoring.h"
#include "AddonScoring.h"
#include "upgradescoring.h"
#include "builderdata.h"
#include <memory>

class productionmanager
{
private:

	buildingplacer buildplacer;
	std::vector<buildingtimedata> buildingstomake;
	expansionbuilder myexpandbuilder;
	std::vector<std::shared_ptr<UtilityFarm>> myfarms;
	std::shared_ptr<workingmanager> workers;
	buildingmanager builmanager;
	std::set<BWAPI::Unit> buildings;
	int bankedminerals;
	int bankedgas;

	std::vector<builderdata> builders;

	float correctsupply;
	float supplyset;
	float productionpower;
	int unittrainers;
	int maxtrainers;
	float buildordersupply;

	bool supplycheck(int supply);
	bool unitproducer(BWAPI::UnitType type);
	BWAPI::Unit returnbuilding(std::vector<BWAPI::Unit> possiblebuild, buildinfo myinfo);
	bool checkresources(int minerals, int gas);
	bool buildordercheck(int minerals, int gas, int firstminerals, int firstgas);
	BWAPI::Unit getaddonbuilding(BWAPI::UnitType addonbuilding);
	void emptybuildingcheck();
	void threatresetcheck();

public:

	productionmanager();
	~productionmanager()
	{
		workers.reset();
	}
	void Addunit(BWAPI::Unit u);
	void addcompleted(BWAPI::Unit u);
	void deleteunit(BWAPI::Unit u);
	void build();
	void scoreexpansition();
	void calculatePpower();
	void update();
	void whattobuild();
	void buildersdone();
	int gettobebuiltcount(BWAPI::UnitType lookfor);
	bool addoncheck(BWAPI::UnitType addonbuilding);
	bool freequeue(BWAPI::UnitType unit);


	std::set<BWAPI::Unit> returnworkers();
	std::set<BWAPI::Unit> returnAWorkers();

	std::shared_ptr<workingmanager> returnworkermanager()
	{
		return workers;
	}
	float getsupplyset()
	{
		return supplyset;
	}
	float getcorrectsupply()
	{
		return correctsupply;
	}
	float getPpower()
	{
		return productionpower;
	}
	int returnunittrainers()
	{
		return unittrainers;
	}
	int returnmaxunittrainers()
	{
		return maxtrainers;
	}
};