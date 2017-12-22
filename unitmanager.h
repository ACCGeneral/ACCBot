#pragma once

#include "BWAPI.h"
#include <set>
#include "Worldinformation.h"
#include "influenceMapData.h"
#include "utilityfunction.h"
#include "usefulFunctions.h"
#include "scandata.h"
#include "states.h"

using namespace BWAPI;

bool attackcheck(BWAPI::UnitType attacking, BWAPI::UnitType attacked);
bool attackAir(BWAPI::UnitType attacking);
bool attackGround(BWAPI::UnitType attacking);
bool attackingcheck(BWAPI::UnitType ucheck);


class meleemanager;
class rangedmanager;
class MedicManger;
class detectorManagaer;
class SegieManager;

class unitmanager
{
private:

	meleemanager *mymelee;
	rangedmanager *myranged;
	MedicManger *mymedics;
	SegieManager *myseigetanks;
	detectorManagaer *mydetectors;

	BWAPI::Unit attackbuilding;

	std::vector<scandata> currentscans;

	std::set<BWAPI::Unit> myunits;
	std::set<BWAPI::Unit> myworkers;
	std::set<BWAPI::Unit> enemyunits;
	std::set<BWAPI::Unit> scanners;

	BWAPI::Position rallypoint;
	BWAPI::Position attackPosition;
	BWAPI::Position defendPosition;
	BWAPI::Position regroupPosition;
	BWAPI::Position attackgrouposition;

	float attackingscore;
	float builduparmyscore;

	float airattackscore;
	float groundattackscore;

	state mystate;

	BWAPI::Position lastposccan;
	float scancooldown;

	int figherworkersneeded;

	void firstmove(BWAPI::Unit unit);
	void recalculatescores();

	void defencecheck();
	void workerdefence();

	void clockdefence();
	void scanupdate();

	void rerallycheck();

	bool nearbasecheck(BWAPI::Unit unit);

	bool unitnearclocked(BWAPI::Unit hideunit);

	BWAPI::Unit ClosestUnitAttack(BWAPI::Unit unit);

	int basepoint;

	int rallytime;

	bool attacking;

	int resetrallytime;


public:

	unitmanager();
	~unitmanager()
	{
		delete mymelee;
		delete myranged;
		delete mymedics;
	}

	void update();

	void addunit(BWAPI::Unit unit);
	void ondeath(BWAPI::Unit unit);

	void addenemyunit(BWAPI::Unit unit);
	void removeenemyunit(BWAPI::Unit unit);

	bool visablecheck(std::set<BWAPI::Unit> units);

	BWAPI::Unit findenemybuilding();

	std::set<BWAPI::Unit> returnenemyunits()
	{
		return enemyunits;
	}

	std::set<BWAPI::Unit> returnmyunits()
	{
		return myunits;
	}

	BWAPI::Position returncurrentstaterally()
	{
		if (mystate == attack || mystate == searchanddestroy)
		{
			return attackPosition;
		}
		else if (mystate == defend)
		{
			return defendPosition;
		}
		else if (mystate == GroupUpAttack)
		{
			return attackgrouposition;
		}
		else
		{
			return rallypoint;
		}

	}

	int returnrallytime()
	{
		return rallytime;
	}
	
};

