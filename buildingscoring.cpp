#include "buildingscoring.h"
#include "productionmanager.h"

buildingscore::buildingscore(productionmanager *p) : UtilityFarm(p)
{
	expandcooldown = 0;
	gascooldown = 0;
	expandbool = false;
	gasbool = false;
}

buildinfo buildingscore::scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder)
{
	float productvalue = pro->getPpower();
	float aggrolevel = Worldinformation::getinstance().aggressionlevel;
	counterlist counterdata = Worldinformation::getinstance().returncounterscores();

	buildinfo newbuild = current;

	if ((Broodwar->self()->supplyTotal()/2) != 200)
	{
		supplyscore = clamp(1 / (1 + (std::pow((2.718 * 4.0f), ((pro->getcorrectsupply() / pro->getsupplyset()) - 0.8f)*-13.0f))));

		if (supplyscore > currentscore)
		{
			currentscore = supplyscore;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Supply_Depot;
		}
	}

	if (gascooldown == false && gasbool == false)
	{
		float workers = pro->returnworkermanager()->getcurrentworkernum() + getfromproduction(buildorder, BWAPI::UnitTypes::Terran_SCV);

		int test = pro->returnworkermanager()->returngasses();
		gasscore = clamp(std::fmin(std::pow(((workers + (0 * 0.4)) * (std::fmin(pro->returnworkermanager()->returngasses(), 1.0f)) / (16.0f * std::fmax(pro->returnworkermanager()->gasnum(), 1))), 2.5f), 1));
		if (gasscore > currentscore)
		{
			currentscore = gasscore;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Refinery;
		}
	}

	if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Command_Center))
	{
		barrecksscore = std::fmin(std::pow(((productvalue / 10.0f)), 0.6f), 1.0f);
		float counterweight = clamp(logisticscore(0.7f,counterdata.percantageoftop3(BWAPI::UnitTypes::Terran_Barracks),0.5,-10));
		barrecksscore = scoringfixer(barrecksscore, counterweight);
		float numberweight = getcountbuilding(BWAPI::UnitTypes::Terran_Barracks, buildorder);
		numberweight /= pro->returnmaxunittrainers();
		numberweight = invert(numberweight, 1);
		numberweight = linearandquadratic(0.0f, numberweight, 0.8f);
		barrecksscore = scoringfixer(barrecksscore, numberweight);

		if (getcountbuilding(BWAPI::UnitTypes::Terran_Barracks, buildorder) == 0)
		{
			barrecksscore = std::fmin(barrecksscore * newtechbonus, 1);
		}

		if (barrecksscore > currentscore)
		{
			currentscore = barrecksscore;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Barracks;
		}

		enginscore;
		bool major = false;


		if (Worldinformation::getinstance().returnclock() && getcountbuilding(BWAPI::UnitTypes::Terran_Engineering_Bay, buildorder) == 0)
		{
			enginscore = 1.1f;
			major = true;
		}
		else
		{
			float engnum = getcountbuilding(BWAPI::UnitTypes::Terran_Engineering_Bay, buildorder);

			if (engnum >= 2)
			{
				enginscore = 0.0f;
			}
			else
			{
				float unitseffected = Worldinformation::getinstance().myunits->returnINFunits();
				unitseffected /= 40.0f;
				enginscore = clamp(logisticscore(1.0f, unitseffected, 0.6, -10));
				if (engnum == 0)
				{
					enginscore = std::fmin(enginscore * newtechbonus, 1);
				}
			}
		}

		if (enginscore > currentscore)
		{
			currentscore = enginscore;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Engineering_Bay;
			if (major)
			{
				newbuild.major = true;
			}
		}

	}

	

	if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Engineering_Bay))
	{
		turretscore = 0;
		bool major = false;

		if (Worldinformation::getinstance().returnclock() && getcountbuilding(BWAPI::UnitTypes::Terran_Missile_Turret, buildorder) == 0)
		{
			turretscore = 1.1f;
			major = true;
		}
		else
		{
			float turrentsneeded = ((Worldinformation::getinstance().returnenemy()->returnairunits()) - (getcountbuilding(BWAPI::UnitTypes::Terran_Missile_Turret, buildorder) * 8.0f));
			turrentsneeded = clamp(turrentsneeded / 8.0f);
			turretscore = linearandquadratic(0.0, turrentsneeded, 2.5f);
		}

		if (turretscore > currentscore)
		{
			currentscore = turretscore;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Missile_Turret;
			if (major)
			{
				newbuild.major = true;
			}
		}

	}

	
	if (pro->returnworkermanager()->gasnum() > 0)
	{
		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Barracks))
		{
			if (getcountbuilding(BWAPI::UnitTypes::Terran_Academy,buildorder) == 0)
			{
				float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Firebat) / counterdata.gethigherst());
				counterscore = logisticscore(0.8, counterscore, 0.6, -9);
				float medicscore = Worldinformation::getinstance().myunits->getcount(BWAPI::UnitTypes::Terran_Marine) + Worldinformation::getinstance().myunits->getcount(BWAPI::UnitTypes::Terran_Firebat);
				float medics = (Worldinformation::getinstance().myunits->getcount(BWAPI::UnitTypes::Terran_Medic) + getfromproduction(buildorder, BWAPI::UnitTypes::Terran_Medic)) * 6;
				medicscore = (medicscore - medics) / 6.0f;
				medicscore = linearandquadratic(0.0, medicscore, 2.0f);
				acadamyscore = fmax(medicscore, counterscore);

				acadamyscore = std::fmin(acadamyscore * newtechbonus, 1);

				if (acadamyscore > currentscore)
				{
					currentscore = acadamyscore;
					newbuild.thistype = newbuild.Building;
					newbuild.mytype = BWAPI::UnitTypes::Terran_Academy;
				}
			}

			factoryscore = std::fmin(std::pow(((productvalue / 10.0f)), 0.6f), 1.0f);
			float counterweight = clamp(logisticscore(0.7f, counterdata.percantageoftop3(BWAPI::UnitTypes::Terran_Factory), 0.5, -10));
			factoryscore = scoringfixer(factoryscore, counterweight);
			float numberweight = getcountbuilding(BWAPI::UnitTypes::Terran_Factory, buildorder);
			numberweight /= pro->returnmaxunittrainers();
			numberweight = invert(numberweight, 1);
			numberweight = linearandquadratic(0.0f, numberweight, 0.8f);
			factoryscore = scoringfixer(factoryscore, numberweight);

			if (getcountbuilding(BWAPI::UnitTypes::Terran_Factory, buildorder) < 1)
			{
				factoryscore = std::fmin(factoryscore * newtechbonus, 1);
			}

			if (factoryscore > currentscore)
			{
				currentscore = factoryscore;
				newbuild.thistype = newbuild.Building;
				newbuild.mytype = BWAPI::UnitTypes::Terran_Factory;
			}
		}

		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Factory))
		{
			/*starport = std::fmin(std::pow(((productvalue / 10.0f)), 0.6f), 1.0f);
			float counterweight = clamp(logisticscore(0.7f, counterdata.percantageoftop3(BWAPI::UnitTypes::Terran_Starport), 0.5, -10));
			starport = scoringfixer(starport, counterweight);
			float numberweight = getcountbuilding(BWAPI::UnitTypes::Terran_Starport, buildorder);
			numberweight /= pro->returnmaxunittrainers();
			numberweight = invert(numberweight, 1);
			numberweight = linearandquadratic(0.0f, numberweight, 2.5f);
			starport = scoringfixer(starport, numberweight);

			if (getcountbuilding(BWAPI::UnitTypes::Terran_Starport, buildorder) == 0)
			{
				starport = std::fmin(starport * newtechbonus, 1);
			}

			if (starport > currentscore)
			{
				currentscore = starport;
				newbuild.thistype = newbuild.Building;
				newbuild.mytype = BWAPI::UnitTypes::Terran_Starport;
			}*/

			float armoryscore;

			float armnum = getcountbuilding(BWAPI::UnitTypes::Terran_Armory, buildorder);

			if (armnum >= 2)
			{
				armoryscore = 0.0f;
			}
			else
			{
				float unitseffected = Worldinformation::getinstance().myunits->returntanks();
				unitseffected /= 30.0f;
				armoryscore = clamp(logisticscore(1.0f, unitseffected, 0.6, -10));
				if (armnum == 0)
				{
					float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Goliath) / counterdata.gethigherst());
					counterscore = logisticscore(0.8, counterscore, 0.6, -9);
					armoryscore = fmax(counterscore, armoryscore);
					armnum = std::fmin(armoryscore * newtechbonus, 1);
				}
			}

			if (armoryscore > currentscore)
			{
				currentscore = armoryscore;
				newbuild.thistype = newbuild.Building;
				newbuild.mytype = BWAPI::UnitTypes::Terran_Armory;
			}


		}


		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Starport))
		{
			sciencefasility = 0;
			float counterbattle = (counterdata.getscore(BWAPI::UnitTypes::Terran_Battlecruiser) / counterdata.gethigherst());
			counterbattle = logisticscore(0.8, counterbattle, 0.6, -9);
			float countervalkyrie = (counterdata.getscore(BWAPI::UnitTypes::Terran_Valkyrie) / counterdata.gethigherst());
			countervalkyrie = logisticscore(0.8, counterbattle, 0.6, -9);
			float counterscience = (counterdata.getscore(BWAPI::UnitTypes::Terran_Science_Vessel) / counterdata.gethigherst());
			counterscience = logisticscore(0.8, counterbattle, 0.6, -9);
			float sciencefasilityscore = fmax(fmax(counterbattle, countervalkyrie), counterscience);
			float threat = logisticscore(0.9, invert(Worldinformation::getinstance().threatlevel / 10.0f, 1.0f), 0.7, -10);
			threat = setclamp(threat, 0.5, 1);
			sciencefasility = scoringfixer(threat, sciencefasilityscore);

			if (getcountbuilding(BWAPI::UnitTypes::Terran_Science_Facility, buildorder) >= 2)
			{
				sciencefasility = 0.0f;
			}
			else
			{
				if (getcountbuilding(BWAPI::UnitTypes::Terran_Science_Facility, buildorder) == 0)
				{
					sciencefasility = std::fmin(sciencefasility * newtechbonus, 1);
				}
			}
			if (sciencefasility > currentscore)
			{
				currentscore = sciencefasility;
				newbuild.thistype = newbuild.Building;
				newbuild.mytype = BWAPI::UnitTypes::Terran_Science_Facility;
			}
		}
	}

	if (expandcooldown == false && expandbool == false && influenceMapData::getinstance().returnallleftbase().size() > 0)
	{
		expandscore = 0.0f;
		float threatexspand = clamp(logisticscore(0.9, invert(Worldinformation::getinstance().threatlevel / 10.0f,1.0f), 0.50, -10));
		float mineralneed = clamp(invert(pro->returnworkermanager()->minleft() / (12000.0f * pro->returnworkermanager()->basenum()), 1));
		mineralneed = clamp(linearandquadratic(0.0f, mineralneed, 0.7));
		float gasneed = clamp(invert(pro->returnworkermanager()->gleft() / (5000.0f * std::max(pro->returnworkermanager()->gasnum(),1)), 1));
		gasneed = clamp(linearandquadratic(0.0f, gasneed, 0.7));
		expandscore = std::fmax(threatexspand, std::fmax(mineralneed, gasneed));
		if (expandscore > currentscore)
		{
			currentscore = expandscore;
			newbuild.thistype = newbuild.Building;
			newbuild.mytype = BWAPI::UnitTypes::Terran_Command_Center;
		}

	}

	return newbuild;
}


void buildingscore::update()
{
	if (gascooldown == true)
	{
		int seconds = Broodwar->getFrameCount() / 24;
		if (seconds - lastsecondgas >= 30)
		{
			gascooldown = false;
		}

	}

	if (expandcooldown == true)
	{
		int seconds = Broodwar->getFrameCount() / 24;
		if (seconds - lastsecondexpand >= 200)
		{
			expandcooldown = false;
		}
	}

}


void buildingscore::setcooldown(BWAPI::UnitType type)
{
	if (type.isResourceDepot())
	{
		expandbool = false;
		expandcooldown = true;
		int seconds = Broodwar->getFrameCount() / 24;
		lastsecondexpand = seconds;
	}
	else if (type.isRefinery())
	{
		gasbool = false;
		int seconds = Broodwar->getFrameCount() / 24;
		lastsecondgas = seconds;
		gascooldown = true;
	}
}

void buildingscore::setcooldownbool(BWAPI::UnitType type)
{
	if (type.isResourceDepot())
	{
		expandbool = true;
	}
	else if (type.isRefinery())
	{
		gasbool = true;
	}
}

void buildingscore::resetfromthreat(BWAPI::UnitType type)
{
	if (type.isResourceDepot())
	{
		expandbool = false;
		expandcooldown = false;
	}
	else if (type.isRefinery())
	{
		gasbool = false;
		gascooldown = false;
	}



}