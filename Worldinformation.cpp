#include "Worldinformation.h"
using namespace BWAPI;

Worldinformation::Worldinformation()
{
	clockpossible = false;
	threatlevel = 5;
	enemyarmyscore = 0;
	myarmyscore = 0;
	unitseen = false;
	aggressionlevel = 3;
	threatspike = false;
	oldthreat = 1;

	mybuildings = std::make_shared<buildingcounts>();
	myunits = std::make_shared<UnitCount>();
	enemyunitscounts = std::make_shared<UnitCount>();
}


void Worldinformation::addunit(BWAPI::Unit unit)
{
	if (!unit->getType().isBuilding() && unit->getPlayer() == Broodwar->self())
	{
		if (!unit->getType().isWorker())
		{
			myarmyscore += unit->getType().buildScore();
			myunits->addtocount(unit);
		}
	}
	else if ((unit->getType().isBuilding() || unit->getType().isAddon()) && unit->getPlayer() == Broodwar->self())
	{
		if (unit->getType().isAddon())
		{
			int test = 1;
		}

		Worldinformation::getinstance().mybuildings->addtocount(unit);
	}

}


void Worldinformation::removeunit(BWAPI::Unit removeunit)
{
	if ((removeunit->getType().isBuilding() || removeunit->getType().isAddon()))
	{
		if (removeunit->getPlayer() == Broodwar->self())
		{
			mybuildings->deletecount(removeunit);
		}
		else if (removeunit->getPlayer() == Broodwar->enemy())
		{
			KnownEnemyBuildings.erase(removeunit);
		}
	}

	if (!removeunit->getType().isBuilding())
	{
		if (removeunit->getPlayer() != BWAPI::Broodwar->self())
		{
			KnownEnemyUnits.erase(removeunit);
			enemyunitscounts->deletecount(removeunit);
		
			if (!removeunit->getType().isWorker())
			{
				enemyarmyscore -= removeunit->getType().buildScore();
				counterscoring.removeunit(removeunit->getType());
			}
		}
		else
		{
			if (!removeunit->getType().isWorker())
			{
				myarmyscore -= removeunit->getType().buildScore();
				myunits->deletecount(removeunit);
			}
		}
	}

}

void Worldinformation::onshow(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding() && unit->getPlayer() != BWAPI::Broodwar->self())
	{
		if (unit->getType() == BWAPI::UnitTypes::Protoss_Templar_Archives || unit->getType() == BWAPI::UnitTypes::Terran_Covert_Ops)
		{
			clockpossible = true;
		}

		if (unit->getType().isBuilding() && unit->getPlayer() == Broodwar->enemy())
		{
			KnownEnemyBuildings.insert(unit);
		}
	}

	else if (!unit->getType().isBuilding())
	{
		if (unit->getPlayer() != BWAPI::Broodwar->self())
		{
			if (unit->canCloak() || unit->isCloaked())
			{
				clockpossible = true;
			}
			if (KnownEnemyUnits.find(unit) == KnownEnemyUnits.end())
			{
				KnownEnemyUnits.insert(unit);

				if (unit->getType() != BWAPI::UnitTypes::Zerg_Larva && unit->getType() != BWAPI::UnitTypes::Zerg_Egg)
				{
					enemyunitscounts->addtocount(unit);
				}

				if (!unit->getType().isWorker())
				{
					enemyarmyscore += unit->getType().buildScore();
					counterscoring.addunit(unit->getType());
					unitseen = true;
				}
			}

		}
	}

}

void Worldinformation::onmorph(BWAPI::Unit unit)
{
	if (!unit->getType().isBuilding())
	{
		if (unit->getPlayer() != BWAPI::Broodwar->self() && unit->getType() != BWAPI::UnitTypes::Zerg_Larva && unit->getType() != BWAPI::UnitTypes::Zerg_Egg)
		{
			if (unit->canCloak() || unit->isCloaked())
			{
				clockpossible = true;
			}
			KnownEnemyUnits.insert(unit);
			enemyunitscounts->addtocount(unit);
			
			if (!unit->getType().isWorker())
			{
				enemyarmyscore += unit->getType().buildScore();
				counterscoring.addunit(unit->getType());
				unitseen = true;
			}
		}
	}
}


bool Worldinformation::returnclock()
{
	return clockpossible;
}

void Worldinformation::setclockpossible()
{
	clockpossible = true;
}


void Worldinformation::update()
{

	if (unitseen == false && influenceMapData::getinstance().returnenemybases().size() >= 2)
	{
		unitseen = true;
	}

	if (enemyarmyscore == 0 && unitseen == false)
	{
		threatlevel = 5;
	}
	else
	{
		oldthreat = threatlevel;
		float diff = enemyarmyscore / std::fmax(myarmyscore,1.0f);
		threatlevel = (diff * 5) + aggressionlevel;
		threatlevel = std::fmax(threatlevel, 0);
		threatlevel = std::fmin(threatlevel, 10);

		if (threatlevel >= oldthreat + 5)
		{
			threatspike = true;
		}

	}
}

