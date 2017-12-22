#pragma once

#include <set>
#include "BWAPI.h"
#include "BWTA.h"
#include "buildingcounts.h"
#include "UnitCount.h"
#include <memory>
#include "Counterlist.h"
#include "influenceMapData.h"

class Worldinformation
{
private:

	std::set<BWAPI::Unit> KnownEnemyUnits;
	std::set<BWAPI::Unit> KnownEnemyBuildings;
	bool clockpossible;
	int myarmyscore;
	int enemyarmyscore;
	std::shared_ptr<UnitCount> enemyunitscounts;
	counterlist counterscoring;
	bool unitseen;
	bool threatspike;
	float oldthreat;

public:

	Worldinformation();
	void update();
	void addunit(BWAPI::Unit unit);
	void removeunit(BWAPI::Unit removeunit);
	void onshow(BWAPI::Unit unit);
	void onmorph(BWAPI::Unit unit);

	static Worldinformation &getinstance()
	{
		static Worldinformation myinstance;
		return myinstance;
	}

	bool returnclock();
	void setclockpossible();

	float threatlevel;
	float aggressionlevel;

	std::shared_ptr<buildingcounts> mybuildings;
	std::shared_ptr<UnitCount> myunits;

	std::shared_ptr<UnitCount> returnenemy()
	{
		return enemyunitscounts;
	}
	counterlist returncounterscores()
	{
		return counterscoring;
	}
	void resetthreatspike()
	{
		threatspike = false;
	}
	bool returnthreatspike()
	{
		return threatspike;
	}

	std::set<BWAPI::Unit> returnenemybuildings()
	{
		return KnownEnemyBuildings;
	}

	void cleanup()
	{
		KnownEnemyUnits.clear();
		KnownEnemyBuildings.clear();
		clockpossible = false;
		myarmyscore = 0;
		threatlevel = 5;
		enemyarmyscore = 0;
		unitseen = false;
		threatspike = false;
		oldthreat = 1;

		counterscoring.reset();

		mybuildings.reset();
		myunits.reset();
		enemyunitscounts.reset();

		mybuildings = std::make_shared<buildingcounts>();
		myunits = std::make_shared<UnitCount>();
		enemyunitscounts = std::make_shared<UnitCount>();
	}
};

