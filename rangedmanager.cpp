#include "rangedmanager.h"
#include "unitmanager.h"

void rangedmanager::update(state currentstate)
{
	std::set<BWAPI::Unit> enemyunits = man->returnenemyunits();

	mystate = currentstate;

	if (mystate == defend)
	{
		for (auto unit : rangedunits)
		{
			int test = Broodwar->elapsedTime() % 2;

			if (unit->isIdle() || test == 0)
			{
				BWAPI::Unit currenttarget = unit->getTarget();
				BWAPI::Unit attacker = ClosestUnitAttack(unit);
				if (attacker == NULL)
				{
					unit->attack(man->returncurrentstaterally());
				}
				else if ((currenttarget != NULL && attacker != currenttarget) || currenttarget == NULL)
				{
					unit->attack(attacker);
				}
			}
			else if (unit->getTarget() != NULL)
			{
				if (!unit->getTarget()->canAttack() && unit->isUnderAttack())
				{
					unit->attack(ClosestUnitAttack(unit));
				}
			}
		}
	}
	else if (mystate == GroupUpAttack)
	{
		BWAPI::Position gotopos = man->returncurrentstaterally();

		for (auto unit : rangedunits)
		{
			if (unit->getPosition().getDistance(gotopos) > 200 && unit->getTarget() == NULL)
			{
				unit->attack(gotopos);
			}

		}
	}
	else if (mystate == attack)
	{
		for (auto unit : rangedunits)
		{
			int test = Broodwar->elapsedTime() % 2;

			if (unit->isIdle() || test == 0)
			{
				BWAPI::Unit currenttarget = unit->getTarget();
				BWAPI::Unit attacker = ClosestUnitAttack(unit);
				if (attacker == NULL && currenttarget == NULL)
				{
					unit->attack(man->returncurrentstaterally());
				}
				else if ((currenttarget != NULL && attacker != currenttarget) || currenttarget == NULL)
				{
					unit->attack(attacker);
				}
			}
			else if (unit->getTarget() != NULL)
			{
				if (!unit->getTarget()->canAttack() && unit->isUnderAttack())
				{
					unit->attack(ClosestUnitAttack(unit));
				}
			}
		}

	}

	else if (mystate == searchanddestroy)
	{
		BWAPI::Unit enbuilding = man->findenemybuilding();

		std::set<BWAPI::Unit> enemybuildings = Worldinformation::getinstance().returnenemybuildings();

		if (enemybuildings.size() == 0 || !man->visablecheck(enemybuildings))
		{
			std::set<BWTA::BaseLocation*> bases = influenceMapData::getinstance().returnallleftbase();

			std::set<BWTA::BaseLocation*>::iterator it = bases.begin();

			if (basepoint >= bases.size())
			{
				basepoint = 0;
			}

			std::advance(it, basepoint);

			for (auto unit : rangedunits)
			{
				if (unit->getDistance((*it)->getPosition()) < 200)
				{
					if (basepoint < bases.size() + 1)
					{
						basepoint++;
					}
					else
					{
						basepoint = 0;
					}
				}
				if (unit->isIdle())
				{
					unit->attack((*it)->getPosition());
				}
			}

		}
		else if (enbuilding != NULL)
		{
			basepoint = 0;
			for (auto unit : rangedunits)
			{
				if (unit->isIdle())
				{
					unit->attack(enbuilding->getPosition());
				}
			}
		}

	}

}


void rangedmanager::retreatcall()
{
	for (auto units : rangedunits)
	{
		units->move(man->returncurrentstaterally());
	}

}


BWAPI::Unit rangedmanager::ClosestUnitAttack(BWAPI::Unit unit)
{
	BWAPI::Unit attackenemy = NULL;
	float edistance = 100000;

	for (auto enemy : Broodwar->enemy()->getUnits())
	{
		if (attackingcheck(enemy->getType()))
		{
			if (enemy->isVisible() && unit->getDistance(enemy) < edistance && attackcheck(unit->getType(), enemy->getType()))
			{
				attackenemy = enemy;
				edistance = unit->getDistance(enemy);
			}
		}

	}

	return attackenemy;

}
