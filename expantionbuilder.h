#include "BWAPI.h"
#include "BWTA.h"

using namespace BWAPI;

struct expansionbuilder
{
	Unit builder;
	TilePosition expandtile;
	UnitType HQ;
	bool active;
};