#pragma once

#include "BWAPI.h"
#include "BWTA.h"

class influenceMapData
{
private:

	std::set<BWTA::BaseLocation*> startinglocations;
	std::set<BWTA::BaseLocation*> leftoverbaselocations;
	std::set<BWTA::Region*>	allRegions;
	std::set<BWTA::BaseLocation*> enemybases;
	std::set<BWTA::BaseLocation*> mybases;
	int bassesnumber;

	BWAPI::Position defaultrally;
	BWAPI::Position attackrally;

	BWTA::BaseLocation *mystart;
	BWTA::BaseLocation *mainenemybase;

	float maxdistancebase;

public:

	influenceMapData();

	float returnmaxdis()
	{
		return maxdistancebase;
	}

	BWAPI::Position returnidelrally()
	{
		return defaultrally;
	}


	BWAPI::Position returnattackrally()
	{
		return attackrally;
	}

	BWTA::BaseLocation *returnmystart()
	{
		return mystart;
	}
	BWTA::BaseLocation *returnmainenemybase()
	{
		return mainenemybase;
	}
	std::set<BWTA::BaseLocation*> returnmybases()
	{
		return mybases;
	}

	std::set<BWTA::BaseLocation*> returnstarlocation()
	{
		return startinglocations;
	}
	std::set<BWTA::BaseLocation*> returnallleftbase()
	{
		return leftoverbaselocations;
	}
	std::set<BWTA::BaseLocation*> returnenemybases()
	{
		return enemybases;
	}
	static influenceMapData &getinstance()
	{
		static influenceMapData myinstance;
		return myinstance;
	}
	void setupworldinfo();

	void addenemyarea(BWAPI::Unit u);
	void removeenemyarea(BWAPI::Unit u);
	void addmybase(BWAPI::Unit u);
	void removemybase(BWAPI::Unit u);
	BWAPI::TilePosition closestbasetobuilder(BWAPI::Unit u);
	BWAPI::Position closestenemybase();
	BWAPI::Position closestfreebasetoenemy();
	bool notonbasecheck(BWAPI::TilePosition buildpos);


	void cleanup()
	{
		bassesnumber = 1;
		maxdistancebase = 0;
		startinglocations.clear();
		leftoverbaselocations.clear();
		allRegions.clear();
		enemybases.clear();
		mybases.clear();
		mystart = NULL;
		mainenemybase = NULL;

	}

};

