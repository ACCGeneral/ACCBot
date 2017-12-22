#include "workermanager.h"
using namespace BWAPI;


void workingmanager::updateAssets()
{
	for (int i = 0; i < gasses.size(); i++)
	{
		int test = gasses[i]->refinery->getResources();

		if (gasses[i]->refinery->getResources() <= 0)
		{
			for (auto workers : gasses[i]->gasworkers)
			{
				workers->stop(false);
				freeworkers.insert(workers);
			}

			gasses[i]->gasworkers.clear();
			gasses.erase(gasses.begin() + i);
			i--;
			totalworkersneeded -= 3;
		}
	}

	for (int i = 0; i < basses.size(); i++)
	{
		std::set<BWAPI::Unit>::iterator it;

		for (it = basses[i]->mineralsnear.begin(); it != basses[i]->mineralsnear.end();)
		{
			int test = (*it)->getResources();

			if ((*it)->getResources() <= 0 || (*it)->getHitPoints() <= 0)
			{
				basses[i]->mineralsnear.erase(it++);
				totalworkersneeded -= 2;
			}
			else
			{
				it++;
			}
		}

		if (basses[i]->mineralsnear.size() <= 0)
		{
			for (auto worker : basses[i]->mineralworkers)
			{
				worker->stop(false);
			}
			totalworkersneeded -= 2;
			mineralworkrs -= basses[i]->mineralworkers.size();
			basses[i]->mineralworkers.clear();
			basses.erase(basses.begin() + i);
			i--;
		}
	}

}

workingmanager::workingmanager()
{
	totalworkersneeded = 0; 
	avaliablegasses = 0;
}

void workingmanager::Addunit(BWAPI::Unit u)
{
	if (u->getType().isWorker() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		allworkers.insert(u);
	}
	if (u->getType().isRefinery())
	{
		avaliablegasses--;
	}
}

void workingmanager::deleteunit(BWAPI::Unit u)
{
	if (u->getType().isWorker() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		allworkers.erase(u);
		freeworkers.erase(u);
		removecheck(u);
	}

	else if (u->getType() == Broodwar->self()->getRace().getRefinery() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		for (int i = 0; i < gasses.size(); i++)
		{
			if (gasses[i]->refinery == u)
			{
				for (auto L : gasses[i]->gasworkers)
				{
					L->stop(false);
					freeworkers.insert(L);
				}
				gasses[i]->gasworkers.clear();
				gasdata *temp = gasses[i];
				gasses.erase(gasses.begin() + i);
				delete temp;
				avaliablegasses++;
				break;
			}

		}
	}

	else if (u->getBuildType().isResourceDepot() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		for (int i = 0; i < basses.size(); i++)
		{
			if (basses[i]->Base == u)
			{
				for (auto L : basses[i]->mineralworkers)
				{
					L->stop(false);
				}
				basses[i]->mineralworkers.clear();
				BaseData *temp = basses[i];
				basses.erase(basses.begin() + i);
				delete temp;
				break;

			}
		}
	}

}

void workingmanager::addcompletedunit(BWAPI::Unit u)
{
	if (u->getType().isWorker() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		freeworkers.insert(u);
	}
	if (u->getType() == Broodwar->self()->getRace().getRefinery() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		gasdata *newgas = new gasdata();
		newgas->maxworkers = 3;
		newgas->refinery = u;
		gasses.push_back(newgas);
		totalworkersneeded += 3;
	}
	if (u->getType().isResourceDepot() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		BaseData *newbase = new BaseData();
		newbase->Base = u;

		for (auto min : Broodwar->getMinerals())
		{
			if (min->getDistance(u) < 200)
			{
				newbase->mineralsnear.insert(min);
			}
		}

		for (auto gas : Broodwar->getGeysers())
		{
			if (u->getDistance(gas) < 200)
			{
				avaliablegasses++;
			}
		}

		newbase->mineralcount = newbase->mineralsnear.size();
		totalworkersneeded += newbase->mineralcount * 2;
		totalworkersneeded += 2;
		basses.push_back(newbase);
	}
	

}

std::set<BWAPI::Unit> workingmanager::returnAllW()
{
	return allworkers;
}

std::set<BWAPI::Unit> workingmanager::returnAvaliableW()
{
	return freeworkers;
}

void workingmanager::update()
{
	mineralsleft = 0;
	gasleft = 0;


	for (int i = 0; i < basses.size(); i++)
	{
		for (auto mins : basses[i]->mineralsnear)
		{
			mineralsleft += mins->getResources();
		}
	}
	std::set<BWTA::BaseLocation*> mybases = influenceMapData::getinstance().returnmybases();

	for (auto gasg : mybases)
	{
		gasleft += gasg->gas();
	}

	gartheringworkers();
}

void workingmanager::gartheringworkers()
{
	updateAssets();
	assignIdle();

	for (int i = 0; i < gasses.size(); i++)
	{
		std::set<BWAPI::Unit>::iterator iter;

		for (iter = gasses[i]->gasworkers.begin(); iter != gasses[i]->gasworkers.end();)
		{
			if ((*iter)->getHitPoints() <= 0)
			{
				gasses[i]->gasworkers.erase(*iter++);
			}
			else
			{
				if ((((*iter)->isCarryingGas() || (*iter)->isCarryingMinerals()) && (*iter)->isIdle()) && !(*iter)->isAttacking())
				{
					(*iter)->returnCargo();
				}
				else if (((*iter)->isIdle() || (*iter)->isGatheringMinerals()) && !(*iter)->isAttacking())
				{
					(*iter)->rightClick(gasses[i]->refinery, false);
				}

				iter++;
			}
		}
	}

	for (int i = 0; i < basses.size(); i++)
	{
		std::set<BWAPI::Unit>::iterator iter;

		for (iter = basses[i]->mineralworkers.begin(); iter != basses[i]->mineralworkers.end();)
		{
			if ((*iter)->getHitPoints() <= 0)
			{
				basses[i]->mineralworkers.erase(*iter++);
			}
			else
			{
				if (((*iter)->isCarryingGas() || (*iter)->isCarryingMinerals()) && (*iter)->isIdle())
				{
					(*iter)->rightClick(basses[i]->Base);
				}
				else if ((*iter)->isIdle() || (*iter)->isGatheringGas())
				{
					getclosestmineral(*iter, basses[i]->mineralsnear);
				}

				iter++;
			}
		}
	}

}

void workingmanager::getclosestmineral(BWAPI::Unit worker, std::set<BWAPI::Unit> baseminerals)
{
	BWAPI::Unit closestmin = NULL;

	for (auto minerals : baseminerals)
	{
		if (closestmin == NULL)
		{
			closestmin = minerals;
		}
		else
		{
			if (worker->getDistance(minerals) < worker->getDistance(closestmin))
			{
				closestmin = minerals;
			}
		}
	}
	if (closestmin != NULL)
	{
		worker->rightClick(closestmin);
	}
}

void workingmanager::assignIdle()
{
	for (std::set<BWAPI::Unit>::iterator it = freeworkers.begin(); it != freeworkers.end();)
	{
		bool foundjob = false;

		if (scoregasses((*it)))
		{
			foundjob = true;
			freeworkers.erase(it++);
		}
		else if (foundjob == false && ((*it)->isIdle() || (*it)->isGatheringGas()) && !(*it)->isGatheringMinerals())
		{
			foundjob = scorebasses((*it));
			it++;
		}
		else if (foundjob == false)
		{
			it++;
		}
	}
	
}

void workingmanager::removecheck(BWAPI::Unit worker)
{
	for (int i = 0; i < basses.size(); i++)
	{
		basses[i]->mineralworkers.erase(worker);
	}
	for (int i = 0; i < gasses.size(); i++)
	{
		gasses[i]->gasworkers.erase(worker);
	}

}

void workingmanager::inuse(BWAPI::Unit u)
{
	for (int i = 0; i < basses.size(); i++)
	{
		basses[i]->mineralworkers.erase(u);
	}
	for (int i = 0; i < gasses.size(); i++)
	{
		gasses[i]->gasworkers.erase(u);
	}

	freeworkers.erase(u);
}

void workingmanager::addtofree(BWAPI::Unit u)
{
	if (u->getPlayer() == BWAPI::Broodwar->self())
	{
		freeworkers.insert(u);
	}
	
}

bool workingmanager::scorebasses(BWAPI::Unit u)
{
	float score = -10000;
	BaseData *mybase = NULL;

	if (basses.size() == 1)
	{
		removecheck(u);
		basses[0]->mineralworkers.insert(u);
		return true;
	}
	else
	{
		for (int i = 0; i < basses.size(); i++)
		{
			if (basses[i]->mineralworkers.size() < basses[i]->mineralcount * 2)
			{
				float lenght = u->getDistance(basses[i]->Base);
				float lenghscore = invert(linearandquadratic(0.0f, lenght / 2000, 2.5), 1);
				float saturationscore = invert(linearandquadratic(0.0f, basses[i]->mineralworkers.size() / (basses[i]->mineralcount * 2), 0.7), 1);
				float totalscore = clamp(scoringfixer(lenghscore, saturationscore));

				if (totalscore >= score)
				{
					score = totalscore;
					mybase = basses[i];
				}
			}

		}
		if (mybase != NULL)
		{
			removecheck(u);
			mineralworkrs++;
			mybase->mineralworkers.insert(u);
			return true;
		}
	}


	return false;
}

bool workingmanager::scoregasses(BWAPI::Unit u)
{
	float score = -10000;
	gasdata *mygas = NULL;

	if (gasses.size() == 1)
	{
		if (gasses[0]->gasworkers.size() < gasses[0]->maxworkers)
		{
			removecheck(u);
			gasses[0]->gasworkers.insert(u);
			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		for (int i = 0; i < gasses.size(); i++)
		{
			if (gasses[i]->gasworkers.size() < gasses[i]->maxworkers)
			{
				float lenght = u->getDistance(gasses[i]->refinery);
				float lenghscore = invert(linearandquadratic(0.0f, lenght / 2000, 1.0), 1);

				float saturationscore = invert(linearandquadratic(0.0f, gasses[i]->gasworkers.size() / gasses[i]->maxworkers, 1.6), 1);
				saturationscore *= 0.6;

				float totalscore = clamp(lenghscore + saturationscore / 2);

				if (totalscore >= score)
				{
					score = totalscore;
					mygas = gasses[i];
				}
			}
		}

		if (mygas != NULL)
		{
			removecheck(u);
			mygas->gasworkers.insert(u);
			return true;
		}
	}

	return false;

}

BWAPI::Unit workingmanager::getworker()
{
	if (freeworkers.size() > 0)
	{
		BWAPI::Unit Runit = *freeworkers.begin();
		inuse(Runit);
		return Runit;
	}

	return NULL;
}



