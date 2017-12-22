#include "UtilityFarm.h"
#include "productionmanager.h"

UtilityFarm::UtilityFarm(productionmanager *p)
{
	pro = p;
	newtechbonus = 1.3;
}

int UtilityFarm::getcountbuilding(BWAPI::UnitType type, std::vector<buildinfo> buildorder)
{
	int count = 0;
	count = Worldinformation::getinstance().mybuildings->getcount(type) + getfromproduction(buildorder, type) + pro->gettobebuiltcount(type);

	return count;
}

int UtilityFarm::getcountUnit(BWAPI::UnitType type, std::vector<buildinfo> buildorder)
{
	int count = 0;
	count = Worldinformation::getinstance().myunits->getcount(type) + getfromproduction(buildorder, type) + pro->gettobebuiltcount(type);

	return count;

}

