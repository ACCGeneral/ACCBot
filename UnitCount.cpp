#include "UnitCount.h"
using namespace BWAPI;


UnitCount::UnitCount()
{
	INFunits = 0;
	tanks = 0;
	airunits = 0;
}

void UnitCount::addtocount(BWAPI::Unit type)
{
	if (type->getType().whatBuilds().first == BWAPI::UnitTypes::Terran_Barracks)
	{
		INFunits++;
	}
	if (type->getType().whatBuilds().first == BWAPI::UnitTypes::Terran_Factory)
	{
		tanks++;
	}
	if (type->getType().whatBuilds().first == BWAPI::UnitTypes::Terran_Starport)
	{
		airunits++;
	}

	for (int i = 0; i < unitcounts.size(); i++)
	{
		if (unitcounts[i].first == type->getType())
		{
			unitcounts[i].second++;
			return;
		}
	}
	std::pair<BWAPI::UnitType, int> toadd;
	toadd.first = type->getType();
	toadd.second = 1;

	unitcounts.push_back(toadd);
}

int UnitCount::getcount(BWAPI::UnitType type)
{
	for (int i = 0; i < unitcounts.size(); i++)
	{
		if (unitcounts[i].first == type)
		{
			return unitcounts[i].second;
		}
	}

	return 0;
}

void UnitCount::deletecount(BWAPI::Unit type)
{
	if (type->getType().whatBuilds().first == BWAPI::UnitTypes::Terran_Barracks)
	{
		INFunits--;
	}
	if (type->getType().whatBuilds().first == BWAPI::UnitTypes::Terran_Factory)
	{
		tanks--;
	}
	if (type->getType().whatBuilds().first == BWAPI::UnitTypes::Terran_Starport)
	{
		airunits--;
	}

	for (int i = 0; i < unitcounts.size(); i++)
	{
		if (unitcounts[i].first == type->getType())
		{
			unitcounts[i].second--;
			return;
		}
	}
}

int UnitCount::numbertecheffects(BWAPI::TechType ability)
{
	int unitswithablity = 0;

	for (int i = 0; i < unitcounts.size(); i++)
	{
		if (unitcounts[i].first.abilities().contains(ability))
		{
			unitswithablity += unitcounts[i].second;
		}
	}

	return unitswithablity;
}
