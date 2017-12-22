#pragma once

#include "BWAPI.h"
#include "gasdata.h"
#include "BaseData.h"
#include "BWTA.h"
#include "influenceMapData.h"
#include "usefulFunctions.h"
#include "utilityfunction.h"

//design is loosly based on the MooseBot worker manager design by Adam montgomerie https://github.com/iarfmoose/MooseBot/blob/master/MooseBot/MooseBot/WorkerManager.h

class workingmanager
{
private:

	std::set<BWAPI::Unit> allworkers;
	std::set<BWAPI::Unit> freeworkers;
	std::vector<gasdata*> gasses;
	std::vector<BaseData*> basses;

	void updateAssets();
	void gartheringworkers();
	void getclosestmineral(BWAPI::Unit worker, std::set<BWAPI::Unit> baseminerals);
	void assignIdle();
	void removecheck(BWAPI::Unit worker);
	bool scorebasses(BWAPI::Unit u);
	bool scoregasses(BWAPI::Unit u);

	int totalworkersneeded;
	int avaliablegasses;

	int mineralsleft;
	int gasleft;

	int mineralworkrs;

public:

	workingmanager();
	void Addunit(BWAPI::Unit u);
	void deleteunit(BWAPI::Unit u);
	void addcompletedunit(BWAPI::Unit u);
	std::set<BWAPI::Unit> returnAllW();
	std::set<BWAPI::Unit> returnAvaliableW();
	void update();
	void inuse(BWAPI::Unit u);
	void addtofree(BWAPI::Unit u);
	BWAPI::Unit getworker();

	int getneededworkers()
	{
		return totalworkersneeded;
	}
	int getcurrentworkernum()
	{
		return allworkers.size();
	}
	int returngasses()
	{
		return avaliablegasses;
	}
	int minleft()
	{
		return mineralsleft;
	}
	int gleft()
	{
		return gasleft;
	}
	int returnminworkers()
	{
		return mineralworkrs;
	}
	int basenum()
	{
		return basses.size();
	}
	int gasnum()
	{
		return gasses.size();
	}




};