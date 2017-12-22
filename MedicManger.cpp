#include "MedicManger.h"

MedicManger::MedicManger(unitmanager *unitman)
{

	man = unitman;
	healcheck = 5;
	lasthealcheck = Broodwar->elapsedTime();
	basepoint = 0;
}

void MedicManger::update(state currentstate)
{

	mystate = currentstate;

	if (mystate == idel)
	{
		for (auto unit : medics)
		{
			int test = Broodwar->elapsedTime() % 10;

			if (unit->isIdle() && test == 0)
			{
				BWAPI::Unit wonded = NULL;
				float bestscore = 0.01;

				for (auto healableunits : healable)
				{
					float newscore = scoreunit(healableunits, currentstate, unit);
					if (newscore > bestscore)
					{
						bestscore = newscore;
						wonded = healableunits;
					}
				}
				if (wonded == NULL)
				{
					unit->move(man->returncurrentstaterally());
				}
			}
		}
	}
	else if (mystate == GroupUpAttack)
	{
		BWAPI::Position gotopos = man->returncurrentstaterally();

		for (auto unit : medics)
		{
			if (unit->getPosition().getDistance(gotopos) > 200)
			{
				unit->move(gotopos);
			}

		}
	}
	else if (mystate == defend)
	{
		for (auto unit : medics)
		{
			int test = Broodwar->elapsedTime() % 3;

			if (unit->isIdle() || test == 0)
			{
				BWAPI::Unit wonded = NULL;
				float bestscore = 0.01;

				for (auto healableunits : healable)
				{
					float newscore = scoreunit(healableunits, currentstate, unit);
					if (newscore > bestscore)
					{
						bestscore = newscore;
						wonded = healableunits;
					}
				}

				if (wonded == NULL)
				{
					unit->move(man->returncurrentstaterally());
				}

				else
				{
					unit->useTech(BWAPI::TechTypes::Healing, wonded);
				}
			}
		}
	}
	else if (mystate == attack)
	{
		for (auto unit : medics)
		{
			int test = Broodwar->elapsedTime() % 3;

			if (unit->isFollowing() || unit->isIdle() || test == 0)
			{
				BWAPI::Unit wonded = NULL;
				float bestscore = 0.01;

				for (auto healableunits : healable)
				{
					float newscore = scoreunit(healableunits, currentstate, unit);
					if (newscore > bestscore)
					{
						bestscore = newscore;
						wonded = healableunits;
					}
				}

				if (wonded == NULL)
				{
					BWAPI::Position pos = getclosestnonmedic(unit);

					if (pos != BWAPI::Position(0,0))
					{
						unit->move(pos);
					}
					else
					{
						unit->move(man->returncurrentstaterally());
					}
					
				}
				else
				{
					unit->useTech(BWAPI::TechTypes::Healing, wonded);
				}
			}
		}

	}

	else if (mystate == searchanddestroy)
	{
		std::set<BWAPI::Unit> enemybuildings = Worldinformation::getinstance().returnenemybuildings();

		if (enemybuildings.size() == 0)
		{
			std::set<BWTA::BaseLocation*> bases = influenceMapData::getinstance().returnallleftbase();

			std::set<BWTA::BaseLocation*>::iterator it = bases.begin();

			if (basepoint >= bases.size())
			{
				basepoint = 0;
			}

			std::advance(it, basepoint);

			for (auto unit : medics)
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
					unit->move((*it)->getPosition());
				}
			}

		}
		else
		{
			basepoint = 0;
			for (auto unit : medics)
			{
				if (unit->isIdle())
				{
					unit->move(man->returncurrentstaterally());
				}
			}
		}

	}


}

float MedicManger::scoreunit(BWAPI::Unit wantedunit, float currentscore, BWAPI::Unit medic)
{
	if (wantedunit->getType().maxHitPoints() == 0)
	{
		return 0.0f;
	}

	float normalizedhp = (float)(wantedunit->getHitPoints() / wantedunit->getType().maxHitPoints());
	normalizedhp = invert(normalizedhp, 1);

	float healscore = logisticscore(1,normalizedhp,0.5,-10);
	healscore = clamp(healscore);
	if (healscore <= 0.01f)
	{
		return 0.0f;
	}
	float normalizeddistance = invert(clamp(std::abs(medic->getDistance(wantedunit->getPosition())) / 4000.0f),1);
	float distance = clamp(linearandquadratic(0.0,normalizeddistance,0.8));
	float totalscore = scoringfixer(distance, healscore);

	return totalscore;

}

BWAPI::Position MedicManger::getclosestnonmedic(BWAPI::Unit me)
{
	std::set<BWAPI::Unit> myunits = man->returnmyunits();

	for (auto unit : myunits)
	{
		if (unit != NULL && unit != me && unit->getType() != BWAPI::UnitTypes::Terran_Medic)
		{
			return unit->getPosition();
		}
	}

	return BWAPI::Position(0, 0);

}

void MedicManger::retreatcall()
{
	for (auto units : medics)
	{
		units->move(man->returncurrentstaterally());
	}

}