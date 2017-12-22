#include "scoutingmanager.h"
using namespace BWAPI;

scoutingmanager::scoutingmanager(productionmanager *p)
{
	freescouter = NULL;
	enemyscouter = NULL;

	scoutenemybasetime.push_back(60);
	scoutfreebasestime.push_back(100);

	pro = p;
	currectbase = 0;
}

void scoutingmanager::update()
{

	if (freescouter != NULL && freescouter->getHitPoints() <= 0)
	{
		freescouter = NULL;
	}
	else if (freescouter != NULL && (currectbase >= influenceMapData::getinstance().returnallleftbase().size() || influenceMapData::getinstance().returnallleftbase().size() == 0))
	{
		finderlogic(freescouter);
		freescouter = NULL;
	}
	else if (freescouter != NULL)
	{
		std::set<BWTA::BaseLocation*> bases = influenceMapData::getinstance().returnallleftbase();
		std::set<BWTA::BaseLocation*>::iterator it = bases.begin();
		std::advance(it, currectbase);

		if (freescouter->isIdle())
		{
			freescouter->move((*it)->getPosition());
		}
		else if (freescouter->getPosition().getDistance((*it)->getPosition()) < 200)
		{
			currectbase++;
		}
	}

	if (enemyscouter != NULL && enemyscouter->getHitPoints() <= 0)
	{
		enemyscouter = NULL;
		int seconds = Broodwar->getFrameCount() / 24;
		scoutenemybasetime.push_back(seconds + 40);
	}
	else if (enemyscouter != NULL)
	{
		enemyscoutstate(enemyscouter);
	}


	if (enemyscouter == NULL && scoutenemybasetime.size() > 0)
	{
		int seconds = Broodwar->getFrameCount() / 24; 
		if (seconds >= scoutenemybasetime[0])
		{
			if (enemyscouter == NULL && pro->returnworkermanager()->returnAvaliableW().size() > 5)
			{
				enemyscouter = pro->returnworkermanager()->getworker();
				enemyscouter->stop();
				findenemybase(enemyscouter);
				scoutenemybasetime.erase(scoutenemybasetime.begin());
			}
			enmeyscoutstate = moving;
		}
	}

	if (scoutfreebasestime.size() > 0)
	{
		int seconds = Broodwar->getFrameCount() / 24;
		if (seconds >= scoutfreebasestime[0])
		{
			if (freescouter == NULL && pro->returnworkermanager()->returnAvaliableW().size() > 5)
			{
				currectbase = 0;
				freescouter = pro->returnworkermanager()->getworker();
				freescouter->stop();
				scoutfreebasestime.push_back(scoutfreebasestime[0] + 200);
				scoutfreebasestime.erase(scoutfreebasestime.begin());
			}

		}
	}
	
}


void scoutingmanager::scoutbaselocations(BWAPI::Unit finder)
{
	if (finder == NULL)
	{
		return;
	}

	int firstlook = 0;

	for (auto emptybases : influenceMapData::getinstance().returnallleftbase())
	{
		if (firstlook == 0)
		{
			finder->move(emptybases->getPosition(), false);
			firstlook++;
		}
		else
		{
			finder->move(emptybases->getPosition(), true);
		}
	}


}

void scoutingmanager::findenemybase(BWAPI::Unit scouter)
{
	if (scouter == NULL)
	{
		return;
	}

	int firstlook = 0;

	if (influenceMapData::getinstance().returnenemybases().size() == 0)
	{
		for (auto basestarts : influenceMapData::getinstance().returnstarlocation())
		{
			if (basestarts->getRegion() != BWTA::getStartLocation(Broodwar->self())->getRegion())
			{
				if (firstlook == 0)
				{
					scouter->move(basestarts->getPosition(), false);
					firstlook++;
				}
				else
				{
					scouter->move(basestarts->getPosition(), true);
				}
			}
		}
	}
	else
	{
		for (auto enemybases : influenceMapData::getinstance().returnenemybases())
		{
			if (enemybases != *influenceMapData::getinstance().returnenemybases().begin())
			{
				if (firstlook == 0)
				{
					scouter->move(enemybases->getPosition(), false);
					firstlook++;
				}
				else
				{
					scouter->move(enemybases->getPosition(), true);
				}
			}
		}

		BWTA::BaseLocation *mainbase = *influenceMapData::getinstance().returnenemybases().begin();
		if (mainbase != NULL)
		{
			scouter->move(mainbase->getPosition(), false);
		}
	}



}

void scoutingmanager::finderlogic(BWAPI::Unit finder)
{
	if (finder != NULL)
	{
		finder->stop();
		pro->returnworkermanager()->addtofree(finder);
	}
}

void scoutingmanager::enemyscoutstate(BWAPI::Unit scouter)
{
	if (enmeyscoutstate == moving)
	{
		if (scouter->isIdle() && influenceMapData::getinstance().returnenemybases().size() > 0)
		{
			enmeyscoutstate = movearound;
		}
		else if (scouter->isIdle())
		{
			findenemybase(scouter);
		}
	}
	else if (enmeyscoutstate == movearound)
	{

		if (influenceMapData::getinstance().returnenemybases().size() > 0)
		{
			BWTA::BaseLocation *mainbase = influenceMapData::getinstance().returnmainenemybase();

			if (scouter->isIdle())
			{
				BWAPI::Position enamypos = mainbase->getPosition();

				int xmove = (rand() % 800 + 1) - 400;
				int ymove = (rand() % 800 + 1) - 400;
				scouter->move(enamypos + BWAPI::Position(xmove, ymove));
			}

			if (scouter->getHitPoints() <= 10 && (scouter->getRegion() == Broodwar->getRegionAt(mainbase->getPosition())))
			{
				int seconds = Broodwar->getFrameCount() / 24;
				scoutenemybasetime.push_back(seconds + 60);
				if (scouter != NULL)
				{
					pro->returnworkermanager()->addtofree(scouter);
					scouter = NULL;
				}
			}
		}
	}


}


