#pragma once

#include "BWAPI.h"
#include <vector>

struct counterdata
{
private:

	BWAPI::UnitType unit;

	std::vector<BWAPI::UnitType> hardcounters;
	std::vector<BWAPI::UnitType> counteredby;

	float score;

public:

	counterdata(BWAPI::UnitType mytype)
	{
		unit = mytype;
		score = 0;
	}

	void sethardandsoft(std::vector<BWAPI::UnitType> hard, std::vector<BWAPI::UnitType> soft)
	{
		hardcounters = hard;
		counteredby = soft;
	}

	void checkscore(BWAPI::UnitType unit)
	{
		for (int i = 0; i < hardcounters.size(); i++)
		{
			if (hardcounters[i] == unit)
			{
				score += 5;
				return;
			}
		}

		for (int i = 0; i < counteredby.size(); i++)
		{
			if (counteredby[i] == unit)
			{
				score += 1;
				return;
			}
		}
	
		score += 4;

	}


	void removescore(BWAPI::UnitType unit)
	{
		for (int i = 0; i < hardcounters.size(); i++)
		{
			if (hardcounters[i] == unit)
			{
				score -= 5;
				return;
			}
		}

		for (int i = 0; i < counteredby.size(); i++)
		{
			if (counteredby[i] == unit)
			{
				score -= 1;
				return;
			}
		}

		score -= 4;

	}

	float returnscore()
	{
		return score;
	}

	BWAPI::UnitType returntype()
	{
		return unit;
	}

	void reset()
	{
		score = 0;
	}

};