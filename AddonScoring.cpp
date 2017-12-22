#include "AddonScoring.h"
#include "productionmanager.h"

AddonScoring::AddonScoring(productionmanager *p) : UtilityFarm(p)
{

	
}

buildinfo AddonScoring::scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder)
{
	buildinfo newbuild = current;
	counterlist counterdata = Worldinformation::getinstance().returncounterscores();

	if (pro->addoncheck(BWAPI::UnitTypes::Terran_Science_Facility) && getcountbuilding(BWAPI::UnitTypes::Terran_Physics_Lab,buildorder) == 0)
	{
		physicslabscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Battlecruiser) / counterdata.gethigherst());
		float threat = logisticscore(0.9, invert(Worldinformation::getinstance().threatlevel / 10.0f, 1.0f), 0.5, -10);
		threat = setclamp(threat, 0.6, 1);
		physicslabscore = scoringfixer(physicslabscore, threat);
		physicslabscore = logisticscore(0.8, physicslabscore, 0.6, -9);
		physicslabscore = std::fmin(physicslabscore * newtechbonus, 1);

		if (physicslabscore > currentscore)
		{
			currentscore = physicslabscore;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Physics_Lab;
		}

	}
	if (pro->addoncheck(BWAPI::UnitTypes::Terran_Science_Facility) && getcountbuilding(BWAPI::UnitTypes::Terran_Covert_Ops,buildorder) == 0)
	{
		covertops = (counterdata.getscore(BWAPI::UnitTypes::Terran_Ghost) / counterdata.gethigherst());
		covertops = logisticscore(0.8, covertops, 0.6, -9);
		float threat = logisticscore(0.9, invert(Worldinformation::getinstance().threatlevel / 10.0f, 1.0f), 0.5, -10);
		threat = setclamp(threat, 0.6, 1);
		covertops = scoringfixer(covertops, threat);
		covertops = std::fmin(covertops * newtechbonus, 1);

		if (covertops > currentscore)
		{
			currentscore = covertops;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Covert_Ops;
		}
	}

	if (pro->addoncheck(BWAPI::UnitTypes::Terran_Factory) && pro->gettobebuiltcount(BWAPI::UnitTypes::Terran_Machine_Shop) == 0)
	{
		machineshop = (counterdata.getscore(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) / counterdata.gethigherst());
		machineshop = logisticscore(0.8, machineshop, 0.6, -9);

		if (getcountbuilding(BWAPI::UnitTypes::Terran_Machine_Shop, buildorder) == 0)
		{
			machineshop = std::fmin(machineshop * newtechbonus, 1);
		}

		if (machineshop > currentscore)
		{
			currentscore = machineshop;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Machine_Shop;
		}

	}

	if (pro->addoncheck(BWAPI::UnitTypes::Terran_Starport) && pro->gettobebuiltcount(BWAPI::UnitTypes::Terran_Control_Tower) == 0)
	{
		Ctower = (counterdata.getscore(BWAPI::UnitTypes::Terran_Science_Vessel) / counterdata.gethigherst());
		Ctower = logisticscore(0.8, Ctower, 0.6, -9);
		float Valkyries = (counterdata.getscore(BWAPI::UnitTypes::Terran_Valkyrie) / counterdata.gethigherst());
		Valkyries = logisticscore(0.8, Ctower, 0.6, -9);
		float battle = (counterdata.getscore(BWAPI::UnitTypes::Terran_Battlecruiser) / counterdata.gethigherst());
		battle = logisticscore(0.8, Ctower, 0.6, -9);
		Ctower = std::fmax(std::fmax(Ctower, Valkyries), battle);
		float threat = logisticscore(0.9, invert(Worldinformation::getinstance().threatlevel / 10.0f, 1.0f), 0.5, -10);
		threat = setclamp(threat, 0.6, 1);
		Ctower = scoringfixer(Ctower, threat);

		if (getcountbuilding(BWAPI::UnitTypes::Terran_Control_Tower, buildorder) == 0)
		{
			Ctower = std::fmin(Ctower * newtechbonus, 1);
		}

		if (Ctower > currentscore)
		{
			currentscore = Ctower;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Control_Tower;
		}

	}

	if (pro->addoncheck(BWAPI::UnitTypes::Terran_Command_Center) && Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Academy) && pro->gettobebuiltcount(BWAPI::UnitTypes::Terran_Comsat_Station) == 0)
	{
		bool major = false;
		comstower = 0.0f;

		if (Worldinformation::getinstance().returnclock() && getcountbuilding(BWAPI::UnitTypes::Terran_Comsat_Station, buildorder) == 0)
		{
			comstower = 1.2f;
			major = true;
		}
		else
		{
			comstower = 0.8f;
		}

		if (getcountbuilding(BWAPI::UnitTypes::Terran_Comsat_Station, buildorder) == 0)
		{
			comstower = std::fmin(comstower * newtechbonus, 1);
		}

		if (comstower > currentscore)
		{
			currentscore = comstower;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Comsat_Station;
			newbuild.major = major;
		}

	}


	return newbuild;
}


void AddonScoring::resetfromthreat(BWAPI::UnitType type)
{




}