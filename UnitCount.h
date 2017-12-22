#pragma once

#include "BWAPI.h"
#include "BWTA.h"
#include <vector>

struct UnitCount
{
private:
	std::vector<std::pair<BWAPI::UnitType, int>> unitcounts;
	int INFunits;
	int airunits;
	int tanks;


public:

	UnitCount();
	void addtocount(BWAPI::Unit type);
	int getcount(BWAPI::UnitType type);
	void deletecount(BWAPI::Unit type);
	int numbertecheffects(BWAPI::TechType ability);


	int returnINFunits()
	{
		return INFunits;
	}
	int returnairunits()
	{
		return airunits;
	}
	int returntanks()
	{
		return tanks;
	}

};