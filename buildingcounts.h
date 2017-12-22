#pragma once

#include "BWAPI.h"
#include "BWTA.h"
#include <vector>
#include "numberoff.h"

struct buildingcounts
{
public:

	std::vector<std::pair<BWAPI::UnitType, nums>> buildingcount;

	buildingcounts();
	void addtocount(BWAPI::Unit type);
	void addtocompleted(BWAPI::Unit type);
	int getcount(BWAPI::UnitType type);
	void deletecount(BWAPI::Unit type);
	bool hasbuilding(BWAPI::UnitType type);
	bool hascomplete(BWAPI::UnitType type);
	int getcompletecount(BWAPI::UnitType type);

};