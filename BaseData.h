#pragma once
#include "BWAPI.h"

struct BaseData
{
	BWAPI::Unit Base;

	std::set<BWAPI::Unit> mineralworkers;
	std::set<BWAPI::Unit> mineralsnear;

	int mineralcount;

};