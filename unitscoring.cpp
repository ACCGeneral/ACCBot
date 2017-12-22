#include "unitscoring.h"
#include "productionmanager.h"


unitscoring::unitscoring(productionmanager *p) : UtilityFarm(p)
{


}

buildinfo unitscoring::scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder)
{
	buildinfo build = current;
	float aggrolevel = Worldinformation::getinstance().aggressionlevel;
	float threatlevel = setclamp(logisticscore(0.9, Worldinformation::getinstance().threatlevel / 10.0f, 0.5, -10),0.4,1);
	counterlist counterdata = Worldinformation::getinstance().returncounterscores();

	if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Command_Center) && (Broodwar->self()->supplyUsed() + BWAPI::UnitTypes::Terran_SCV.supplyRequired() <= 200))
	{
		float currentworkrs = pro->returnworkermanager()->getcurrentworkernum() + getfromproduction(buildorder, BWAPI::UnitTypes::Terran_SCV);

		//workerscore = std::fmin(std::powf(currentworkrs / (float)pro->returnworkermanager()->getneededworkers(), 4.0f), 1);
		workerscore = invert(linearandquadratic(0.0, currentworkrs / (float)pro->returnworkermanager()->getneededworkers(), 4.0f),1.0);

		if (workerscore != 0)
		{
			workerscore = std::fmax(workerscore, 0.5f);
		}

		if (!pro->freequeue(BWAPI::UnitTypes::Terran_SCV))
		{
			workerscore *= 0.9;
		}

		if (workerscore > currentscore)
		{
			build.mytype = BWAPI::UnitTypes::Terran_SCV;
			build.addonname = "";
			currentscore = workerscore;
			build.thistype = build.Unit;
		}
	}

	if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Barracks))
	{
		if (((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Marine.supplyRequired()/2) <= 200))
		{
			float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Marine,buildorder)) / 200.0f, 1);
			numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f),0.5,1);
			marinescore = scoringfixer(threatlevel, numberpen);
			float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Marine) / counterdata.gethigherst());
			counterscore = logisticscore(0.8, counterscore, 0.6, -9);
			marinescore = (scoringfixer(marinescore, counterscore));

			if (!pro->freequeue(BWAPI::UnitTypes::Terran_Marine) || !affordcheck(BWAPI::UnitTypes::Terran_Marine))
			{
				marinescore *= 0.5;
			}

			if (marinescore > currentscore)
			{
				currentscore = marinescore;
				build.thistype = build.Unit;
				build.addonname = "";
				build.mytype = BWAPI::UnitTypes::Terran_Marine;
			}

		}

		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Academy))
		{
			if (((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Medic.supplyRequired()/2) <= 200))
			{
				medicscore = Worldinformation::getinstance().myunits->getcount(BWAPI::UnitTypes::Terran_Marine) + Worldinformation::getinstance().myunits->getcount(BWAPI::UnitTypes::Terran_Firebat);
				float medics = (Worldinformation::getinstance().myunits->getcount(BWAPI::UnitTypes::Terran_Medic) + getfromproduction(buildorder, BWAPI::UnitTypes::Terran_Medic)) * 6;
				medicscore = (medicscore - medics) / 6.0f;
				medicscore = clamp(medicscore);

				medicscore = linearandquadratic(0.0, medicscore, 2.0f);

				if (!pro->freequeue(BWAPI::UnitTypes::Terran_Medic) || !affordcheck(BWAPI::UnitTypes::Terran_Medic))
				{
					medicscore *= 0.5;
				}

				if (medicscore > currentscore)
				{
					currentscore = medicscore;
					build.addonname = "";
					build.thistype = build.Unit;
					build.mytype = BWAPI::UnitTypes::Terran_Medic;
				}
			}

			if (((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Firebat.supplyRequired()/2) <= 200))
			{
				float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Firebat, buildorder)) / 200.0f, 1);
				numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
				firebatscore = scoringfixer(threatlevel, numberpen);
				float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Firebat) / counterdata.gethigherst());
				counterscore = logisticscore(0.8, counterscore, 0.6, -9);
				firebatscore = scoringfixer(firebatscore, counterscore);


				if (!pro->freequeue(BWAPI::UnitTypes::Terran_Firebat) || !affordcheck(BWAPI::UnitTypes::Terran_Firebat))
				{
					firebatscore *= 0.5;
				}

				if (firebatscore > currentscore)
				{
					currentscore = firebatscore;
					build.thistype = build.Unit;
					build.addonname = "";
					build.mytype = BWAPI::UnitTypes::Terran_Firebat;
				}


			}

			if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Covert_Ops) && Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Science_Facility))
			{
				if ((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Ghost.supplyRequired()/2) <= 200)
				{
					float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Ghost, buildorder)) / 200.0f, 1);
					numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
					ghostscore = scoringfixer(threatlevel, numberpen);
					float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Ghost) / counterdata.gethigherst());
					counterscore = logisticscore(0.8, counterscore, 0.6, -9);
					ghostscore = scoringfixer(ghostscore, counterscore);

					if (!pro->freequeue(BWAPI::UnitTypes::Terran_Ghost) || !affordcheck(BWAPI::UnitTypes::Terran_Ghost))
					{
						ghostscore *= 0.5;
					}

					/*if (ghostscore > currentscore)
					{
						currentscore = ghostscore;
						build.addonname = "";
						build.thistype = build.Unit;
						build.mytype = BWAPI::UnitTypes::Terran_Ghost;
					}*/
					
				}
			}
		}
	}

	if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Factory))
	{
		if ((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Vulture.supplyRequired()/2) <= 200)
		{
			float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Vulture, buildorder) * 2) / 200.0f, 1);
			numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
			Vulturescore = scoringfixer(threatlevel, numberpen);
			float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Vulture) / counterdata.gethigherst());
			counterscore = logisticscore(0.8, counterscore, 0.6, -9);
			Vulturescore = scoringfixer(Vulturescore, counterscore);

			if (!pro->freequeue(BWAPI::UnitTypes::Terran_Vulture) || !affordcheck(BWAPI::UnitTypes::Terran_Vulture))
			{
				Vulturescore *= 0.5;
			}

			if (Vulturescore > currentscore)
			{
				currentscore = Vulturescore;
				build.addonname = "";
				build.thistype = build.Unit;
				build.mytype = BWAPI::UnitTypes::Terran_Vulture;
			}


		}

		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Machine_Shop))
		{
			if ((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode.supplyRequired()/2) <= 200)
			{
				float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, buildorder) * 2) / 200.0f, 1);
				numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
				tankscore = scoringfixer(threatlevel, numberpen);
				float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) / counterdata.gethigherst());
				counterscore = logisticscore(0.8, counterscore, 0.6, -9);
				tankscore = scoringfixer(tankscore, counterscore);

				if (!pro->freequeue(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) || !affordcheck(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode))
				{
					tankscore *= 0.5;
				}

				if (tankscore > currentscore)
				{
					currentscore = tankscore;
					build.thistype = build.Unit;
					build.addonname = BWAPI::UnitTypes::Terran_Machine_Shop.getName();
					build.mytype = BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode;
				}
			}
		}

		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Armory))
		{
			if ((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Goliath.supplyRequired()/2) <= 200)
			{
				float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Goliath, buildorder) * 2) / 200.0f, 1);
				numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
				goliathscore = scoringfixer(threatlevel, numberpen);
				float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Goliath) / counterdata.gethigherst());
				counterscore = logisticscore(0.8, counterscore, 0.6, -9);
				goliathscore = scoringfixer(goliathscore, counterscore);

				if (!pro->freequeue(BWAPI::UnitTypes::Terran_Goliath) || !affordcheck(BWAPI::UnitTypes::Terran_Goliath))
				{
					goliathscore *= 0.5;
				}

				if (goliathscore > currentscore)
				{
					currentscore = goliathscore;
					build.thistype = build.Unit;
					build.addonname = "";
					build.mytype = BWAPI::UnitTypes::Terran_Goliath;
				}

			}
		}

	}

	if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Starport))
	{
		if ((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Wraith.supplyRequired()/2) <= 200)
		{
			float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Wraith, buildorder) * 2) / 200.0f, 1);
			numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
			Wraithscore = scoringfixer(threatlevel, numberpen);
			float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Wraith) / counterdata.gethigherst());
			counterscore = logisticscore(0.8, counterscore, 0.6, -9);
			Wraithscore = scoringfixer(Wraithscore, counterscore);

			if (!pro->freequeue(BWAPI::UnitTypes::Terran_Wraith) || !affordcheck(BWAPI::UnitTypes::Terran_Wraith))
			{
				Wraithscore *= 0.5;
			}

			if (Wraithscore > currentscore)
			{
				currentscore = Wraithscore;
				build.thistype = build.Unit;
				build.addonname = "";
				build.mytype = BWAPI::UnitTypes::Terran_Wraith;
			}

		}


		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Science_Facility) && Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Control_Tower))
		{
			if ((Broodwar->self()->supplyUsed() / 2) + (BWAPI::UnitTypes::Terran_Science_Vessel.supplyRequired() / 2) <= 200 && getcountUnit(BWAPI::UnitTypes::Terran_Science_Vessel,buildorder) < 1)
			{
				float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Science_Vessel, buildorder) * 2) / 200.0f, 1);
				numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
				SVscore = scoringfixer(threatlevel, numberpen);
				float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Science_Vessel) / counterdata.gethigherst());
				counterscore = logisticscore(0.8, counterscore, 0.6, -9);
				SVscore = scoringfixer(SVscore, counterscore);

				if (!pro->freequeue(BWAPI::UnitTypes::Terran_Science_Vessel) || !affordcheck(BWAPI::UnitTypes::Terran_Science_Vessel))
				{
					SVscore *= 0.5;
				}

				if (SVscore > currentscore)
				{
					currentscore = SVscore;
					build.thistype = build.Unit;
					build.addonname = BWAPI::UnitTypes::Terran_Control_Tower.getName();
					build.mytype = BWAPI::UnitTypes::Terran_Science_Vessel;
				}
			}

			if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Physics_Lab))
			{
				if ((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Battlecruiser.supplyRequired()/2) <= 200)
				{
					float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Battlecruiser, buildorder) * 6) / 200.0f, 1);
					numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
					Bcscore = scoringfixer(threatlevel, numberpen);
					float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Battlecruiser) / counterdata.gethigherst());
					counterscore = logisticscore(0.8, counterscore, 0.6, -9);
					Bcscore = scoringfixer(Bcscore, counterscore);


					if (!pro->freequeue(BWAPI::UnitTypes::Terran_Battlecruiser) || !affordcheck(BWAPI::UnitTypes::Terran_Battlecruiser))
					{
						Bcscore *= 0.4;
					}

					if (Bcscore > currentscore)
					{
						currentscore = Bcscore;
						build.thistype = build.Unit;
						build.addonname = BWAPI::UnitTypes::Terran_Control_Tower.getName();
						build.mytype = BWAPI::UnitTypes::Terran_Battlecruiser;
					}

				}
			}

		}

		if (Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Control_Tower) && Worldinformation::getinstance().mybuildings->hascomplete(BWAPI::UnitTypes::Terran_Armory))
		{
			if ((Broodwar->self()->supplyUsed()/2) + (BWAPI::UnitTypes::Terran_Valkyrie.supplyRequired()/2) <= 200)
			{
				float numberpen = invert((getcountUnit(BWAPI::UnitTypes::Terran_Valkyrie, buildorder) * 3) / 200.0f, 1);
				numberpen = setclamp(linearandquadratic(0.0f, numberpen, 1.2f), 0.5, 1);
				valkscore = scoringfixer(threatlevel, numberpen);
				float counterscore = (counterdata.getscore(BWAPI::UnitTypes::Terran_Valkyrie) / counterdata.gethigherst());
				counterscore = logisticscore(0.8, counterscore, 0.6, -9);
				valkscore = scoringfixer(valkscore, counterscore);

				if (!pro->freequeue(BWAPI::UnitTypes::Terran_Valkyrie) || !affordcheck(BWAPI::UnitTypes::Terran_Valkyrie))
				{
					valkscore *= 0.5;
				}


				if (valkscore > currentscore)
				{
					currentscore = valkscore;
					build.thistype = build.Unit;
					build.addonname = BWAPI::UnitTypes::Terran_Control_Tower.getName();
					build.mytype = BWAPI::UnitTypes::Terran_Valkyrie;
				}

			}
		}

	}

	return build;
}


bool unitscoring::affordcheck(BWAPI::UnitType checktype)
{
	if (Broodwar->self()->gas() < checktype.gasPrice())
	{
		return false;
	}

	return true;

}

void unitscoring::resetfromthreat(BWAPI::UnitType type)
{




}