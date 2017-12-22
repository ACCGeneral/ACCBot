#include "unitmanager.h"
#include "meleemanager.h"
#include "rangedmanager.h"
#include "MedicManger.h"
#include "SeigeManager.h"
#include "detectorManager.h"

unitmanager::unitmanager()
{
	mymelee = new meleemanager(this);
	myranged = new rangedmanager(this);
	mymedics = new MedicManger(this);
	myseigetanks = new SegieManager(this);
	mydetectors = new detectorManagaer(this);

	mystate = idel;
	rallypoint = BWAPI::Position(0, 0);
	attackbuilding = NULL;
	basepoint = 0;
	groundattackscore = 0;
	figherworkersneeded = 0;
	airattackscore = 0;
	attacking = false;
	resetrallytime = 0;
}

void unitmanager::update()
{
	defencecheck();
	recalculatescores();
	clockdefence();
	scanupdate();
	rerallycheck();


	if (influenceMapData::getinstance().returnmystart() != NULL)
	{
		BWAPI::Position temp = rallypoint;
		rallypoint = influenceMapData::getinstance().returnidelrally();
		if (temp != rallypoint)
		{
			for (auto unit : myunits)
			{
				unit->attack(rallypoint);
			}
		}
		regroupPosition = rallypoint;
	}


	if (mystate == defend && myunits.size() == 0)
	{
		workerdefence();
	}
	

	if (mystate == idel)
	{
		if (attackingscore > builduparmyscore && myunits.size() >= 10)
		{
			if (attackingscore >= 0.8)
			{
				mystate = attack;
			}
			else
			{
				mystate = GroupUp;
				rallytime = Broodwar->elapsedTime();
				attacking = true;
			}

		}
	}
	else if (mystate == attack)
	{
		attackPosition = influenceMapData::getinstance().closestenemybase();

		if ((enemyunits.size() == 0 || !visablecheck(enemyunits)) && influenceMapData::getinstance().returnenemybases().size() == 0)
		{
			mystate = searchanddestroy;
		}
		
		if (builduparmyscore > attackingscore)
		{
			mystate = retreat;
		}
	}
	else if (mystate == retreat)
	{
		mymelee->retreatcall();
		myranged->retreatcall();
		mymedics->retreatcall();
		mydetectors->retreatcall();
		mystate = idel;
	}
	else if (mystate == GroupUp)
	{
		if (Broodwar->elapsedTime() - rallytime > 10)
		{
			mystate = GroupUpAttack;
			rallytime = Broodwar->elapsedTime();
			attackgrouposition = influenceMapData::getinstance().closestfreebasetoenemy();
		}

		if (builduparmyscore > attackingscore)
		{
			mystate = retreat;
		}

	}
	else if (mystate == GroupUpAttack)
	{
		for (auto unit : myunits)
		{
			if (unit->isUnderAttack() && !nearbasecheck(unit))
			{
				mystate = attack;
			}
		}
		if (Broodwar->elapsedTime() - rallytime > 50)
		{
			mystate = attack;
		}
	}
	else if (mystate == searchanddestroy)
	{
		for (auto enemy : enemyunits)
		{
			if (enemy->isVisible())
			{
				mystate = attack;
			}
		}

		if (influenceMapData::getinstance().returnenemybases().size() > 0)
		{
			mystate = attack;
		}

		std::set<BWAPI::Unit> enemybuildings = Worldinformation::getinstance().returnenemybuildings();

		if (enemybuildings.size() == 0 && !visablecheck(enemybuildings))
		{
			std::set<BWTA::BaseLocation*> bases = influenceMapData::getinstance().returnallleftbase();
			if (bases.size() == 0)
			{
				mystate = idel;
			}
		}
	}

	mymelee->update(mystate);
	myranged->update(mystate);
	mymedics->update(mystate);
	mydetectors->update(mystate);
}

void unitmanager::defencecheck()
{

	bool todefend = false;

	for (auto enemy : enemyunits)
	{
		std::set<BWTA::BaseLocation*> mybases = influenceMapData::getinstance().returnmybases();

		for (auto base : mybases)
		{
			float dis = base->getPosition().getDistance(enemy->getPosition());
			if (dis <= 1000)
			{
				for (auto enemysnear : enemyunits)
				{
					float dis = base->getPosition().getDistance(enemysnear->getPosition());
					if (dis <= 600)
					{
						figherworkersneeded += 3;
					}
				}

				defendPosition = enemy->getPosition();
				todefend = true;
				if (mystate != defend)
				{
					for (auto unit : myunits)
					{
						unit->stop();
					}
					mystate = defend;
				}

				break;
			}
		}
	}

	if (todefend == false)
	{
		figherworkersneeded = 0;

		if (mystate == defend)
		{
			mystate = idel;
			mymelee->retreatcall();
			myranged->retreatcall();
			mydetectors->retreatcall();
			mymedics->retreatcall();
		}
	}

}


void unitmanager::addunit(BWAPI::Unit unit)
{
	if (unit->getType().isWorker() == true && unit->getPlayer() == BWAPI::Broodwar->self())
	{
		myworkers.insert(unit);
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Comsat_Station)
	{
		scanners.insert(unit);
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Medic)
	{
		myunits.insert(unit);
		firstmove(unit);
		mymedics->addunit(unit);
	}
	else if (unit->getType().isDetector())
	{
		myunits.insert(unit);
		mydetectors->addunit(unit);
		firstmove(unit);
	}
	/*else if (unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode)
	{
		myunits.insert(unit);
		firstmove(unit);
		myseigetanks->addunit(unit);
		groundattackscore += unit->getType().buildScore();
	}*/
	else if (unit->getPlayer() == BWAPI::Broodwar->self() && !(unit->getType().isBuilding()))
	{
		myunits.insert(unit);
		firstmove(unit);

		if (!unit->getType().isMechanical())
		{
			mymedics->addhealable(unit);
		}
		if (attackAir(unit->getType()))
		{
			airattackscore += unit->getType().buildScore();
		}
		else if (attackGround(unit->getType()))
		{
			groundattackscore += unit->getType().buildScore();
		}

		if (unit->getType().groundWeapon().maxRange() > 2 || unit->getType().airWeapon().maxRange() > 2)
		{
			myranged->addunit(unit);
		}
		else
		{
			mymelee->addunit(unit);
		}

	}

}

void unitmanager::ondeath(BWAPI::Unit unit)
{
	if (unit->getType().isWorker() == true && unit->getPlayer() == BWAPI::Broodwar->self())
	{
		myworkers.erase(unit);
	}
	else if (unit->getPlayer() == BWAPI::Broodwar->self())
	{
		myunits.erase(unit);
		myranged->deleteunit(unit);
		mymelee->deleteunit(unit);
		mymedics->deletehealable(unit);
		mymedics->deleteunit(unit);
		myseigetanks->deleteunit(unit);
		mydetectors->deleteunit(unit);

		if (attackAir(unit->getType()))
		{
			airattackscore -= unit->getType().buildScore();
		}
		else if (attackGround(unit->getType()))
		{
			groundattackscore -= unit->getType().buildScore();
		}
	}

}

void unitmanager::addenemyunit(BWAPI::Unit unit)
{
	if (unit->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon || unit->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony || unit->getType() == BWAPI::UnitTypes::Terran_Bunker || unit->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony)
	{
		enemyunits.insert(unit);
		groundattackscore -= unit->getType().buildScore()/4;
		return;
	}

	if (unit->getPlayer() == BWAPI::Broodwar->enemy() && (!unit->getType().isWorker() && !unit->getType().supplyProvided()))
	{
		if (unit->getType().isBuilding() && (unit->getType() != BWAPI::UnitTypes::Protoss_Photon_Cannon || unit->getType() != BWAPI::UnitTypes::Zerg_Sunken_Colony))
		{
			return;
		}

		if (unit->getType() == BWAPI::UnitTypes::Zerg_Larva || unit->getType() == BWAPI::UnitTypes::Zerg_Egg)
		{
			return;
		}

		else if (!(enemyunits.find(unit) != enemyunits.end()))
		{
			enemyunits.insert(unit);

			if (unit->isFlying())
			{
				airattackscore -= unit->getType().buildScore();
			}
			else
			{
				groundattackscore -= unit->getType().buildScore();
			}
		}
	}

}

void unitmanager::removeenemyunit(BWAPI::Unit unit)
{
	if (unit->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon || unit->getType() == BWAPI::UnitTypes::Zerg_Sunken_Colony || unit->getType() == BWAPI::UnitTypes::Zerg_Spore_Colony || unit->getType() == BWAPI::UnitTypes::Terran_Bunker)
	{
		enemyunits.erase(unit);
		groundattackscore += unit->getType().buildScore() / 4;
		return;
	}
	else if (unit->getPlayer() == BWAPI::Broodwar->enemy() && (!unit->getType().isBuilding() && !unit->getType().isWorker() && !unit->getType().supplyProvided()))
	{
		if (unit->getType() == BWAPI::UnitTypes::Zerg_Larva || unit->getType() == BWAPI::UnitTypes::Zerg_Egg)
		{
			return;
		}

		enemyunits.erase(unit);

		if (unit->isFlying())
		{
			airattackscore += unit->getType().buildScore();
		}
		else
		{
			groundattackscore += unit->getType().buildScore();
		}
	}

}


void unitmanager::firstmove(BWAPI::Unit unit)
{
	if (mystate == idel)
	{
		unit->attack(rallypoint);
	}
	else if (mystate == attack)
	{
		unit->attack(attackPosition);
	}
	else if (mystate == defend)
	{
		unit->attack(defendPosition);
	}
	else if (mystate == retreat)
	{
		unit->move(regroupPosition);
	}
	else if (mystate == GroupUpAttack)
	{
		unit->move(attackgrouposition);
	}
}

void unitmanager::recalculatescores()
{
	if (mystate == defend)
	{
		return;
	}

	float cautionlevel = 0.2;

	float threatattackscore = invert((Worldinformation::getinstance().threatlevel - Worldinformation::getinstance().aggressionlevel) / 10.0f, 1.0f);
	threatattackscore = logisticscore(1.0f, threatattackscore, 0.7, -10);
	float groundscore = ((setclamp(groundattackscore,-2000.0f,2000.0f)) + 2000.0f)/4000.0f;
	groundscore = linearandquadratic(0.0, groundscore,1.3f);
	float airscore = ((setclamp(airattackscore, -2000.0f, 2000.0f)) + 2000.0f) / 4000.0f;
	airscore = linearandquadratic(0.0, airscore, 1.3f);

	attackingscore = scoringfixer(threatattackscore, groundscore);
	attackingscore = scoringfixer(attackingscore, airscore);

	float threatscaredscore = (Worldinformation::getinstance().threatlevel - Worldinformation::getinstance().aggressionlevel) / 10.0f;
	if (mystate != attack)
	{
		threatscaredscore += cautionlevel;
	}
	threatscaredscore = logisticscore(1.0, threatscaredscore, 0.3, -9);
	float scaredgroundscore = ((setclamp(groundattackscore, -2000.0f, 2000.0f)) + 2000.0f) / 4000.0f;
	scaredgroundscore = linearandquadratic(0.0, invert(scaredgroundscore, 1.0f), 0.7f);
	float scaredairscore = ((setclamp(airattackscore, -2000.0f, 2000.0f)) + 2000.0f) / 4000.0f;
	scaredairscore = linearandquadratic(0.0, invert(scaredairscore, 1.0f), 0.7f);

	builduparmyscore = scoringfixer(threatscaredscore, scaredgroundscore);
	builduparmyscore = scoringfixer(builduparmyscore, scaredairscore);

	if (Broodwar->self()->supplyUsed() / 2 >= 180)
	{
		attackingscore += 0.2f;
	}

	if (attacking == true)
	{
		attackingscore += 0.05f;
	}
	else
	{
		builduparmyscore += 0.05f;
	}
}


BWAPI::Unit unitmanager::ClosestUnitAttack(BWAPI::Unit unit)
{
	BWAPI::Unit attackenemy = NULL;
	float distance = 100000;

	for (auto enemy : enemyunits)
	{
		if (unit->isVisible() && unit->getDistance(enemy) < distance && attackcheck(unit->getType(), enemy->getType()))
		{
			attackenemy = enemy;
			distance = unit->getDistance(enemy);
		}
	}

	return attackenemy;
}



void unitmanager::rerallycheck()
{
	bool notclose = false;

	if (mystate == attack && (Broodwar->elapsedTime()) > resetrallytime + 120)
	{
		for (auto unit : myunits)
		{
			if (unit->getDistance(attackPosition) > 500 && unit->canAttack())
			{
				notclose = true;
				break;
			}
		}
	}

	if (notclose && myunits.size() / 2 < enemyunits.size())
	{
		resetrallytime = Broodwar->elapsedTime();
		mystate = GroupUpAttack;
		rallytime = Broodwar->elapsedTime();
		attackgrouposition = influenceMapData::getinstance().closestfreebasetoenemy();
	}


}

void unitmanager::clockdefence()
{
	for (auto enemy : enemyunits)
	{
		if (enemy->isCloaked() || enemy->isBurrowed() && unitnearclocked(enemy))
		{
			for (auto scan : scanners)
			{
				if (scan->getEnergy() >= 50)
				{
					bool inscanprox = false;

					for (int i = 0; i < currentscans.size(); i++)
					{
						if (enemy->getDistance(currentscans[i].scanpos) <= 400.0f)
						{
							inscanprox = true;
						}
					}

					if (inscanprox == false)
					{
						scan->useTech(BWAPI::TechTypes::Scanner_Sweep, enemy->getPosition());
						scandata newscan;
						newscan.scanpos = enemy->getPosition();
						newscan.startsecond = Broodwar->elapsedTime();
						currentscans.push_back(newscan);
					}

				}
			}
		}
	}
}

bool unitmanager::unitnearclocked(BWAPI::Unit hideunit)
{
	for (auto myunit : myunits)
	{
		if (myunit->getDistance(hideunit) <= 400)
		{
			return true;
		}
	}

	return false;

}

void unitmanager::scanupdate()
{
	for (int i = 0; i < currentscans.size(); i++)
	{
		if (Broodwar->elapsedTime() - currentscans[i].startsecond > 10)
		{
			currentscans.erase(currentscans.begin() + i);
			i--;
		}
	}


}


bool unitmanager::visablecheck(std::set<BWAPI::Unit> units)
{
	for (auto unit : units)
	{
		if (unit->isVisible())
		{
			return true;
		}
	}

	return false;
}


void unitmanager::workerdefence() // refernce to https://github.com/iarfmoose/MooseBot/blob/master/MooseBot/MooseBot/ArmyManager.cpp workerdefence function for insperation
{
	int workersattack = std::fmin(figherworkersneeded, myworkers.size());
	int count = 1;

	for (auto worker : myworkers)
	{
		BWAPI::Unit attackthis = ClosestUnitAttack(worker);

		if (attackthis != NULL && attackthis != worker->getTarget())
		{
			worker->attack(attackthis);
		}
		else
		{
			worker->attack(defendPosition);
		}

		count++;

		if (count >= workersattack)
		{
			break;
		}

	}

}

BWAPI::Unit unitmanager::findenemybuilding()
{
	std::set<BWAPI::Unit> enemybuildings = Worldinformation::getinstance().returnenemybuildings();

	for (auto building : enemybuildings)
	{
		if (building->isVisible())
		{
			return building;
		}
	}

	return NULL;

}

bool unitmanager::nearbasecheck(BWAPI::Unit unit)
{
	std::set<BWTA::BaseLocation*> mybases = influenceMapData::getinstance().returnmybases();

	for (auto base : mybases)
	{
		float dis = base->getPosition().getDistance(unit->getPosition());
		if (dis <= 500)
		{
			return true;
		}
	}

	return false;
}


bool attackcheck(BWAPI::UnitType attacking, BWAPI::UnitType attacked)
{
	if ((attacking.airWeapon().targetsAir() || attacking.groundWeapon().targetsAir()) && attacked.isFlyer())
	{
		return true;
	}
	if ((attacking.airWeapon().targetsGround() || attacking.groundWeapon().targetsGround()) && !attacked.isFlyer())
	{
		return true;
	}

	return false;

}

bool attackAir(BWAPI::UnitType attacking)
{
	if ((attacking.airWeapon().targetsAir() || attacking.groundWeapon().targetsAir()))
	{
		return true;
	}
	return false;
}

bool attackGround(BWAPI::UnitType attacking)
{
	if ((attacking.airWeapon().targetsGround() || attacking.groundWeapon().targetsGround()))
	{
		return true;
	}
	return false;
}

bool attackingcheck(BWAPI::UnitType ucheck)
{
	if (!ucheck.isBuilding() || (ucheck == BWAPI::UnitTypes::Protoss_Photon_Cannon || ucheck == BWAPI::UnitTypes::Zerg_Sunken_Colony || ucheck == BWAPI::UnitTypes::Terran_Bunker || ucheck == BWAPI::UnitTypes::Zerg_Spore_Colony))
	{
		if (!ucheck.isWorker() && !ucheck.supplyProvided() && ucheck != BWAPI::UnitTypes::Zerg_Larva && ucheck != BWAPI::UnitTypes::Zerg_Egg)
		{
			return true;
		}
	}

	return false;
}





