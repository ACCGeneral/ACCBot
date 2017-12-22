#include "buildingcounts.h"
using namespace BWAPI;


buildingcounts::buildingcounts()
{

}

void buildingcounts::addtocount(BWAPI::Unit type)
{
	for (int i = 0; i < buildingcount.size(); i++)
	{
		if (buildingcount[i].first == type->getType())
		{
			buildingcount[i].second.numberalive++;
			return;
		}
	}

	std::pair<BWAPI::UnitType, nums> toadd;
	toadd.first = type->getType();
	toadd.second.numberalive = 1;
	buildingcount.push_back(toadd);
}

void buildingcounts::addtocompleted(BWAPI::Unit type)
{
	for (int i = 0; i < buildingcount.size(); i++)
	{
		if (buildingcount[i].first == type->getType())
		{
			buildingcount[i].second.numbercomplete++;
			return;
		}
	}
}


int buildingcounts::getcount(BWAPI::UnitType type)
{
	for (int i = 0; i < buildingcount.size(); i++)
	{
		if (buildingcount[i].first == type)
		{
			return buildingcount[i].second.numberalive;
		}
	}

	return 0;
}

void buildingcounts::deletecount(BWAPI::Unit type)
{
	for (int i = 0; i < buildingcount.size(); i++)
	{
		if (buildingcount[i].first == type->getType())
		{
			buildingcount[i].second.numberalive--;
			if (type->isCompleted())
			{
				buildingcount[i].second.numbercomplete--;
			}
			return;
		}
	}
}

bool buildingcounts::hasbuilding(BWAPI::UnitType type)
{
	for (int i = 0; i < buildingcount.size(); i++)
	{
		if (buildingcount[i].first == type)
		{
			return true;
		}
	}

	return false;
}


bool buildingcounts::hascomplete(BWAPI::UnitType type)
{
	for (int i = 0; i < buildingcount.size(); i++)
	{
		if (buildingcount[i].first == type && buildingcount[i].second.numbercomplete >= 1)
		{
			return true;
		}
	}

	return false;


}

int buildingcounts::getcompletecount(BWAPI::UnitType type)
{

	for (int i = 0; i < buildingcount.size(); i++)
	{
		if (buildingcount[i].first == type)
		{
			return buildingcount[i].second.numbercomplete;
		}
	}

	return 0;


}