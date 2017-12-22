#pragma once
#include <BWAPI.h>
#include <string>
using namespace BWAPI;

//refernce to David Churchill UAlbertaBot MataType class https://github.com/davechurchill/ualbertabot

struct buildinfo
{
public:
	enum Types {Unit = 1, Building, Tech, Upgrade};

	Types thistype;
	BWAPI::UnitType mytype;
	BWAPI::TechType technology;
	BWAPI::UpgradeType upgrade;

	BWAPI::UnitType whatbuildsme;

	int mineralcost;
	int gascost;

	float buildscore;

	bool major;

	std::string addonname;

	buildinfo(Types type)
	{
		thistype = type;
		buildscore = false;
		major = false;
		addonname = "";
	}

	int starttime;

	buildinfo()
	{
		buildscore = false;
		major = false;
	}

	void setup(float score)
	{
		buildscore = score;

		starttime = Broodwar->elapsedTime();

		if (thistype == Unit || thistype == Building)
		{
			mineralcost = mytype.mineralPrice();
			gascost = mytype.gasPrice();

			whatbuildsme = mytype.whatBuilds().first;
		}

		else if (thistype == Upgrade)
		{
			mineralcost = upgrade.mineralPrice();
			gascost = upgrade.gasPrice();

			whatbuildsme = upgrade.whatUpgrades();
		}

		else if (thistype == Tech)
		{
			mineralcost = technology.mineralPrice();
			gascost = technology.gasPrice();

			whatbuildsme = technology.whatResearches();
		}

	}
	
};