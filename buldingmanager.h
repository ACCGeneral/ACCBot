#pragma once

#include "BWAPI.h"
#include "buildinfo.h"


class buildingmanager
{
private:

	std::vector<buildinfo> buildorder;


public:


	buildingmanager()
	{

	}

	std::vector<buildinfo> returntobuild()
	{
		return buildorder;
	}

	void clearbuildorder()
	{
		buildorder.clear();
	}

	void addtobuild(buildinfo added)
	{
		buildorder.push_back(added);
	}

	void addtofront(buildinfo added)
	{
		std::vector<buildinfo>::iterator it;
		it = buildorder.begin();
		it = buildorder.insert(it, added);
	}

	void removefirst()
	{
		buildorder.erase(buildorder.begin());
	}

	void removenum(int num)
	{
		buildorder.erase(buildorder.begin() + num);
	}



};