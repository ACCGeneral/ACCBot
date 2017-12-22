#include "detectorManager.h"

detectorManagaer::detectorManagaer(unitmanager *unitman)
{
	man = unitman;

}


void detectorManagaer::update(state currentstate)
{
	if (mystate == defend)
	{
		for (auto unit : detectors)
		{
			int test = Broodwar->elapsedTime() % 2;

			if (unit->isIdle() || test == 0)
			{
				getclosestclock(unit);
			}
		}
	}
	else if (mystate == GroupUpAttack)
	{
		BWAPI::Position gotopos = man->returncurrentstaterally();

		for (auto unit : detectors)
		{
			unit->move(gotopos);
		}
	}
	else if (mystate == attack)
	{
		for (auto unit : detectors)
		{
			int test = Broodwar->elapsedTime() % 2;

			if (unit->isIdle() || test == 0)
			{
				getclosestclock(unit);
			}
		}
	}

}

void detectorManagaer::retreatcall()
{
	for (auto units : detectors)
	{
		units->move(man->returncurrentstaterally());
	}
}


void detectorManagaer::getclosestclock(BWAPI::Unit mydetector)
{
	for (auto enemy : Broodwar->enemy()->getUnits())
	{
		if (enemy->isBurrowed() || enemy->isCloaked() && enemy->getType() != BWAPI::UnitTypes::Protoss_Observer)
		{
			BWAPI::Position gotopos = enemy->getPosition();

			mydetector->move(gotopos);
		}
	}

}