/* source code by user iarfmoose on GitHub https://github.com/iarfmoose/MooseBot/blob/master/MooseBot/MooseBot/BuildingPlacer.cpp */
#include "buildingplacer.h"
using namespace BWAPI;


buildingplacer::buildingplacer()
{


}

bool buildingplacer::placeBuilding(BWAPI::Unit builder, BWAPI::UnitType buildingtype)
{
	if (builder == NULL)
	{
		return false;
	}

	bool foundspot = false;

	BWAPI::TilePosition buildplace = influenceMapData::getinstance().closestbasetobuilder(builder);

	if (buildingtype.isRefinery())
	{
		return buildgas(builder, buildingtype);
	}
	else
	{
		bool fail = false;
		int count = 0;
		int spiralCount = 1;
		bool isX = true;
		BWAPI::TilePosition shiftPositionX(1, 0);
		BWAPI::TilePosition shiftPositionY(0, 1);

		if (!buildplace.isValid())
		{
			buildplace.makeValid();
		}

		while (!builder->build(buildingtype, buildplace))
		{
			do
			{
				fail = false;

				BWAPI::Unit closestMineral = NULL;

				for (auto mineral : Broodwar->getMinerals())
				{
					if (closestMineral == NULL || buildplace.getDistance(mineral->getTilePosition()) < buildplace.getDistance(closestMineral->getTilePosition()))
					{
						closestMineral = mineral;
					}	
				}
				if (closestMineral != NULL)
				{
					if (buildplace.getDistance(closestMineral->getTilePosition()) < MINERALDIST)
					{
						fail = true;
					}
				}
				for (auto building : Broodwar->self()->getUnits())
				{
					float dis = buildplace.getDistance(building->getTilePosition());
					if (dis <= DISFROMOTHER && building->getType().isBuilding())
					{
						fail = true;
					}
				}
				if (!influenceMapData::getinstance().notonbasecheck(buildplace))
				{
					fail = true;
				}

				//check we aren't blocking gas too
				BWAPI::Unit closestGas = NULL;

				for (auto gas : Broodwar->getGeysers())
				{
					if (closestGas == NULL || buildplace.getDistance(gas->getTilePosition()) < buildplace.getDistance(closestGas->getTilePosition()))
						closestGas = gas;
				}
				if (closestGas != NULL)
				{
					if (buildplace.getDistance(closestMineral->getTilePosition()) < MINERALDIST)
					{
						fail = true;
					}
				}

				if (count % spiralCount == 0)
				{
					if (isX)
					{
						isX = false;
						count = 0;
					}
					else {
						spiralCount++;
						count = 0;
						isX = true;
					}
				}
				count++;
				if (spiralCount % 2 == 0)
				{
					if (isX)
					{
						buildplace -= shiftPositionX;
					}
					else
					{
						buildplace -= shiftPositionY;
					}
				}
				else
				{
					if (isX)
					{
						buildplace += shiftPositionX;
					}
					else
					{
						buildplace += shiftPositionY;
					}
				}
				//search is cut off at SPIRALLIMIT to prevent it from taking too long or placing building to far from approxLocation
				if (spiralCount == SPIRALLIMIT)
				{
					return false;
				}

			} while (fail);
		}


	}

	return true;

}

BWAPI::TilePosition buildingplacer::closesttakenbase(BWAPI::Unit builder)
{
	std::set<BWTA::BaseLocation*> mybase = influenceMapData::getinstance().returnmybases();
	BWAPI::TilePosition closesttile;
	float sortestlenght = 10000000;

	for (auto base : mybase)
	{
		float lenght = builder->getDistance(base->getPosition());

		if (lenght < sortestlenght)
		{
			sortestlenght = lenght;
			closesttile = base->getTilePosition();
		}
	}

	return closesttile;

}

BWAPI::TilePosition buildingplacer::closestfreebase(BWAPI::Unit builder)
{
	BWAPI::Position mypos = influenceMapData::getinstance().returnmystart()->getPosition();
	std::set<BWTA::BaseLocation*> freebases = influenceMapData::getinstance().returnallleftbase();
	BWAPI::TilePosition bestexpandtile = BWAPI::TilePosition(-1, -1);
	float highestscore = -100000;

	for (auto freebase : freebases)
	{
		float score = 0.0f;
		float mineralscore = freebase->minerals() / 12000.0f;
		mineralscore = setclamp(linearandquadratic(0.0,mineralscore,2.0f), 0.4, 1);
		float gasscore = freebase->gas() / 5000.0f;
		gasscore = setclamp(linearandquadratic(0.0f,gasscore,2.0f), 0.4, 1);
		score = scoringfixer(mineralscore, gasscore);
		float lenght = invert(mypos.getDistance(freebase->getPosition()) / influenceMapData::getinstance().returnmaxdis(), 1);
		lenght = logisticscore(0.7, lenght, 0.5, -19);
		score = scoringfixer(score, lenght);

		if (score > highestscore)
		{
			highestscore = score;
			bestexpandtile = freebase->getTilePosition();
		}
	}

	return bestexpandtile;
}

bool buildingplacer::buildgas(BWAPI::Unit builder, BWAPI::UnitType gas)
{
	std::set<BWTA::BaseLocation*> mybases = influenceMapData::getinstance().returnmybases();
	
	for (auto base : mybases)
	{
		BWAPI::Unitset gases = base->getGeysers();

		for (auto gasgy : gases)
		{
			if (builder->build(gas, gasgy->getTilePosition()))
			{
				return true;
			}
		}
	}

	return true;
}

bool buildingplacer::placeAddon(BWAPI::Unit builder, BWAPI::UnitType buildingtype, std::vector<BWAPI::TilePosition> &postried)
{
	if (builder == NULL)
	{
		return false;
	}

	bool closeToMinerals = false;
	bool addonStarted = false;
	bool isX = true;
	int shiftsize = 1;
	bool foundspot = false;


	BWAPI::TilePosition buildPosition = builder->getTilePosition();
	postried.push_back(buildPosition);

		if (builder->buildAddon(buildingtype))
		{
			return true;
		}
		else if (builder->getType() == BWAPI::UnitTypes::Terran_Command_Center)
		{
			return false;
		}
		else 
		{
			while (!addonStarted)
			{
				if (!builder->isLifted())
				{
					builder->lift();
				}

				BWAPI::TilePosition leftshift = buildPosition + BWAPI::TilePosition(-shiftsize,0);
				BWAPI::TilePosition rightshift = buildPosition + BWAPI::TilePosition(shiftsize, 0);
				BWAPI::TilePosition downshift = buildPosition + BWAPI::TilePosition(0, -shiftsize);
				BWAPI::TilePosition upshift = buildPosition + BWAPI::TilePosition(0, shiftsize);
				BWAPI::TilePosition leftup = buildPosition + BWAPI::TilePosition(-shiftsize, shiftsize);
				BWAPI::TilePosition leftdown = buildPosition + BWAPI::TilePosition(-shiftsize, -shiftsize);
				BWAPI::TilePosition rightup = buildPosition + BWAPI::TilePosition(shiftsize, shiftsize);
				BWAPI::TilePosition rightdown = buildPosition + BWAPI::TilePosition(shiftsize, -shiftsize);
			
				if (builder->land(leftshift) && leftshift.isValid() && placementcheck(postried, leftshift) && notnearotherscheck(leftshift, 3))
				{		
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}
				else if (builder->land(rightshift) && rightshift.isValid() && placementcheck(postried, rightshift) && notnearotherscheck(rightshift, 3))
				{
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}
				else if (builder->land(downshift) && downshift.isValid() && placementcheck(postried, downshift) && notnearotherscheck(downshift, 3))
				{
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}
				else if (builder->land(upshift) && upshift.isValid() && placementcheck(postried, upshift) && notnearotherscheck(upshift, 3))
				{
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}
				else if (builder->land(leftup) && leftup.isValid() && placementcheck(postried, leftup) && notnearotherscheck(leftup, 3))
				{
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}
				else if (builder->land(leftdown) && leftdown.isValid() && placementcheck(postried, leftdown) && notnearotherscheck(leftdown, 3))
				{
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}
				else if (builder->land(rightup) && rightup.isValid() && placementcheck(postried, rightup) && notnearotherscheck(rightup, 3))
				{
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}
				else if (builder->land(rightdown) && rightdown.isValid() && placementcheck(postried, rightdown) && notnearotherscheck(rightdown, 3))
				{
					if (builder->buildAddon(buildingtype))
					{
						foundspot = true;
						addonStarted = true;
					}
					return false;
				}

				if (foundspot == true)
				{
					break;
				}

				shiftsize++;


				//search is cut off at SPIRALLIMIT to prevent it from taking too long or placing building to far from approxLocation
				if (shiftsize >= SPIRALLIMIT)
				{
					return false;
				}

			}
		}


	

	return true;
}

bool buildingplacer::expandbuild(BWAPI::Unit builder, BWAPI::UnitType buildingtype, BWAPI::TilePosition expandtile)
{
	if (buildingtype.isResourceDepot() && (builder->getTilePosition().getDistance(expandtile) < 100))
	{
		if (builder->build(buildingtype, expandtile))
		{
			return true;
		}
	}
	return false;
}

bool buildingplacer::placementcheck(std::vector<BWAPI::TilePosition> postried, BWAPI::TilePosition tryingpos)
{
	for (auto pos : postried)
	{
		if (pos == tryingpos)
		{
			return false;
		}
	}

	return true;
}

bool buildingplacer::notnearotherscheck(BWAPI::TilePosition postocheck, float distancefrom)
{
	for (auto building : Broodwar->self()->getUnits())
	{
		float dis = postocheck.getDistance(building->getTilePosition());

		if (dis <= distancefrom && building->getType().isBuilding() && influenceMapData::getinstance().notonbasecheck(postocheck))
		{
			return false;
		}
	}

	return true;

}