#pragma once

#include <set>
#include "BWAPI.h"
#include "CounterData.h"


class counterlist
{
private:

	std::vector<counterdata> units;
	int numofenemyunits;
	int highestscore;
	std::vector<std::pair<int, BWAPI::UnitType>> top3;
	bool defaultscore;
	float totalof3;

public:

	counterlist();
	float getscore(BWAPI::UnitType unit);
	void addunit(BWAPI::UnitType unit);
	void removeunit(BWAPI::UnitType unit);
	void updatadata();
	float percantageoftop3(BWAPI::UnitType buildingproducer);
	int gethigherst()
	{
		return std::fmax(highestscore,1);
	}

	void reset()
	{
		for (int i = 0; i < units.size(); i++)
		{
			units[i].reset();
		}
		numofenemyunits = 0;
	}
};





