#pragma once

#include "BWAPI.h"
#include "BWTA.h"
#include "influenceMapData.h"
#include "usefulFunctions.h"
#include "utilityfunction.h"

#define SPIRALLIMIT 150
#define MINERALDIST 8
#define DISFROMOTHER 6



class buildingplacer
{

public:
	buildingplacer();
	bool placeBuilding(BWAPI::Unit builder, BWAPI::UnitType buildingtype);
	BWAPI::TilePosition closesttakenbase(BWAPI::Unit builder);
	BWAPI::TilePosition closestfreebase(BWAPI::Unit builder);
	bool buildgas(BWAPI::Unit builder, BWAPI::UnitType gas);
	bool placeAddon(BWAPI::Unit builder, BWAPI::UnitType buildingtype, std::vector<BWAPI::TilePosition> &postried);
	bool expandbuild(BWAPI::Unit builder, BWAPI::UnitType buildingtype,BWAPI::TilePosition expandtile);
	bool placementcheck(std::vector<BWAPI::TilePosition> postried, BWAPI::TilePosition tryingpos);
	bool notnearotherscheck(BWAPI::TilePosition postocheck,float distancefrom);

};


//refernce to iarfmoose bot on https://github.com/iarfmoose/MooseBot/blob/master/MooseBot/MooseBot/BuildingPlacer.h, class "BuildingPlacer"