#include "influenceMapData.h"
using namespace BWAPI;

influenceMapData::influenceMapData()
{
	bassesnumber = 1;
	maxdistancebase = 0;
}

void influenceMapData::setupworldinfo()
{
	if (startinglocations.size() == 0 && !BWTA::getStartLocations().empty())
	{
		startinglocations = BWTA::getStartLocations();
		leftoverbaselocations = BWTA::getBaseLocations();
		allRegions = BWTA::getRegions();
	}

	for (auto mybase : startinglocations)
	{
		leftoverbaselocations.erase(mybase);
		if (mybase->getTilePosition() == Broodwar->self()->getStartLocation())
		{
			mystart = mybase;
			mybases.insert(mystart);
		}
	}

	startinglocations.erase(mystart);

	BWAPI::Position mybasepos = mystart->getPosition();

	float maxlenght = -200000;
	float minlenght = 200000;
	BWTA::BaseLocation *closestbase = NULL;

	for (auto bases : BWTA::getBaseLocations())
	{
		float lenght = mybasepos.getDistance(bases->getPosition());

		if (lenght > maxlenght)
		{
			maxlenght = lenght;
		}
		if (lenght < minlenght && bases->getPosition() != mybasepos)
		{
			minlenght = lenght;
			closestbase = bases;
		}
	}

	BWAPI::Position midpos = (closestbase->getRegion()->getCenter() - mybasepos); 
	midpos.x /= 2;
	midpos.y /= 2;

	midpos.x = std::abs(midpos.x);
	midpos.y = std::abs(midpos.y);

	BWAPI::Position test1 = mybasepos - midpos;
	BWAPI::Position test2 = mybasepos + midpos;

	if (test1.getDistance(closestbase->getRegion()->getCenter()) < test2.getDistance(closestbase->getRegion()->getCenter()))
	{
		defaultrally = test1;
	}
	else
	{
		defaultrally = test2;
	}

	maxdistancebase = maxlenght;
}



void influenceMapData::addenemyarea(BWAPI::Unit u)
{
	if ((u->getType().isResourceDepot() || u->isMorphing()) && u->getPlayer() != BWAPI::Broodwar->self())
	{
		std::set<BWTA::BaseLocation*>::iterator it;

		for (it = leftoverbaselocations.begin(); it != leftoverbaselocations.end(); it++)
		{
			BWAPI::TilePosition mytile = (*it)->getTilePosition();

			if (u->getTilePosition().getDistance(mytile) <= 10)
			{
				enemybases.insert(*it);
				if (enemybases.size() == 1)
				{
					mainenemybase = *it;
				}
				leftoverbaselocations.erase(*it++);
				break;
			}
		}

		for (it = startinglocations.begin(); it != startinglocations.end(); it++)
		{
			if (u->getTilePosition() == (*it)->getTilePosition())
			{
				enemybases.insert(*it);
				if (enemybases.size() == 1)
				{
					mainenemybase = *it;
				}

				break;
			}
		}

	}


}

void influenceMapData::removeenemyarea(BWAPI::Unit u)
{
	if ((u->getType().isResourceDepot() || u->isMorphing()) && u->getPlayer() != BWAPI::Broodwar->self())
	{
		std::set<BWTA::BaseLocation*>::iterator it;

		for (it = enemybases.begin(); it != enemybases.end(); it++)
		{
			BWAPI::TilePosition mytile = (*it)->getTilePosition();

			if (u->getTilePosition().getDistance(mytile) <= 10)
			{
				BWTA::BaseLocation* base = *it;

				leftoverbaselocations.insert(*it);
				enemybases.erase(*it);

				if (enemybases.size() == 0)
				{
					break;
				}

				if (mainenemybase == base)
				{
					if (enemybases.size() >= 1)
					{
						mainenemybase = *enemybases.begin();
					}
					else
					{
						mainenemybase = NULL;
					}
				}

				break;
			}
		}
	}


}

void influenceMapData::addmybase(BWAPI::Unit u)
{
	if (u->getType().isResourceDepot() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		std::set<BWTA::BaseLocation*>::iterator it;

		for (it = leftoverbaselocations.begin(); it != leftoverbaselocations.end(); it++)
		{
			if (u->getTilePosition() == (*it)->getTilePosition())
			{
				if (mybases.size() == 1)
				{
					defaultrally = (*it)->getRegion()->getCenter();
				}

				mybases.insert(*it);
				leftoverbaselocations.erase(*it++);
				break;
			}
		}
	}



}

void influenceMapData::removemybase(BWAPI::Unit u)
{
	if (u->getType().isResourceDepot() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		std::set<BWTA::BaseLocation*>::iterator it;

		for (it = mybases.begin(); it != mybases.end(); it++)
		{
			if (u->getTilePosition() == (*it)->getTilePosition())
			{
				leftoverbaselocations.insert(*it);
				mybases.erase(*it++);
				break;
			}
		}
	}
}


BWAPI::TilePosition influenceMapData::closestbasetobuilder(BWAPI::Unit u)
{
	float closest = 1000000;
	BWAPI::TilePosition closestbase = BWAPI::TilePosition(0, 0);

	for (auto base : mybases)
	{
		if (base->getPosition().getDistance(u->getPosition()) < closest)
		{
			closest = base->getPosition().getDistance(u->getPosition());
			closestbase = base->getTilePosition();
		}
	}

	if (closestbase == BWAPI::TilePosition(0, 0))
	{
		closestbase = u->getTilePosition();
	}


	return closestbase;

}

BWAPI::Position influenceMapData::closestfreebasetoenemy()
{
	float closestdis = 10000000;
	BWAPI::Position closestpos;
	BWAPI::Position attackingenemybase = closestenemybase();

	if (mainenemybase != NULL)
	{
		for (auto freebases : leftoverbaselocations)
		{
			if (attackingenemybase.getDistance(freebases->getPosition()) < closestdis)
			{
				closestdis = attackingenemybase.getDistance(freebases->getPosition());
				closestpos = freebases->getPosition();
			}
		}

	}
	else
	{
		closestpos = BWAPI::Position(1000, 1000);
	}

	return closestpos;


}

BWAPI::Position influenceMapData::closestenemybase()
{
	float closest = 1000000;
	BWAPI::Position closestbase = BWAPI::Position(0, 0);

	for (auto base : enemybases)
	{
		if (base->getPosition().getDistance(mystart->getPosition()) < closest)
		{
			closest = base->getPosition().getDistance(mystart->getPosition());
			closestbase = base->getPosition();
		}
	}

	if (closestbase == BWAPI::Position(0, 0))
	{
		closestbase = BWAPI::Position(1000,1000);
	}


	return closestbase;

}

bool influenceMapData::notonbasecheck(BWAPI::TilePosition buildpos)
{
	for (auto freebases : leftoverbaselocations)
	{
		if (freebases->getTilePosition().getDistance(buildpos) <= 6)
		{
			return false;
		}
	}

	return true;
}