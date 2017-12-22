#pragma once

#include "BWAPI.h"

class gasdata
{
public:
	int maxworkers;

	BWAPI::Unit refinery;
	std::set<BWAPI::Unit> gasworkers;

	gasdata();


};