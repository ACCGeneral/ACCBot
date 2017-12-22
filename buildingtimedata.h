#pragma once
#include "BWAPI.h"

struct buildingtimedata
{
	
	BWAPI::UnitType buildingtype;
	int secondstart;
	int secondstowait;
	std::vector<BWAPI::TilePosition> Positionstried;

};