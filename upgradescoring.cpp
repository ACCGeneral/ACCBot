#include "upgradescoring.h"
#include "productionmanager.h"

upgradescoring::upgradescoring(productionmanager *p) : UtilityFarm(p)
{


}


buildinfo upgradescoring::scoring(float &currentscore, buildinfo current, std::vector<buildinfo> buildorder)
{
	buildinfo build = current;

	float threatpunishscore = linearandquadratic(0.0, Worldinformation::getinstance().threatlevel,0.9);

	if (Worldinformation::getinstance().mybuildings->getcompletecount(BWAPI::UnitTypes::Terran_Machine_Shop) > 0)
	{
		if (!Broodwar->self()->isResearching(BWAPI::TechTypes::Tank_Siege_Mode) && !Broodwar->self()->hasResearched(BWAPI::TechTypes::Tank_Siege_Mode))
		{
			int seigecount = getcountUnit(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, buildorder);
			float seigescore = clamp(linearandquadratic(0.0, seigecount / 8.0f, 0.6));
			float seigeupgrade = scoringfixer(seigescore, threatpunishscore);

			if (currentscore < seigeupgrade)
			{
				build.technology = BWAPI::TechTypes::Tank_Siege_Mode;
				build.addonname = "";
				currentscore = seigeupgrade;
				build.thistype = build.Tech;
			}
		}

	}

	if (Worldinformation::getinstance().mybuildings->getcompletecount(BWAPI::UnitTypes::Terran_Engineering_Bay) > 0)
	{
		int unitseffected = getcountUnit(BWAPI::UnitTypes::Terran_Marine, buildorder) + getcountUnit(BWAPI::UnitTypes::Terran_Firebat, buildorder) + getcountUnit(BWAPI::UnitTypes::Terran_Ghost, buildorder);

		if (!Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Weapons) && Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons) != 3 && upgradecheck(BWAPI::UpgradeTypes::Terran_Infantry_Weapons,buildorder))
		{
			if (checkbuildandgame(buildorder, BWAPI::UpgradeTypes::Terran_Infantry_Armor, BWAPI::UnitTypes::Terran_Engineering_Bay))
			{
				float armorscrore = clamp(linearandquadratic(0.0, unitseffected / getnumberscale(BWAPI::UpgradeTypes::Terran_Infantry_Weapons, 20), 0.8));
				armorscrore = scoringfixer(armorscrore, threatpunishscore);

				if (currentscore < armorscrore)
				{
					build.upgrade = BWAPI::UpgradeTypes::Terran_Infantry_Weapons;
					build.addonname = "";
					currentscore = armorscrore;
					build.thistype = build.Upgrade;
				}
			}
		}
		
		if (!Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Armor) && Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor) != 3 && upgradecheck(BWAPI::UpgradeTypes::Terran_Infantry_Armor, buildorder))
		{
			if (checkbuildandgame(buildorder, BWAPI::UpgradeTypes::Terran_Infantry_Weapons, BWAPI::UnitTypes::Terran_Engineering_Bay))
			{
				float attackscore = clamp(linearandquadratic(0.0, unitseffected / getnumberscale(BWAPI::UpgradeTypes::Terran_Infantry_Armor, 20), 0.6));
				attackscore = scoringfixer(attackscore, threatpunishscore);
				if (currentscore < attackscore)
				{
					build.upgrade = BWAPI::UpgradeTypes::Terran_Infantry_Armor;
					build.addonname = "";
					currentscore = attackscore;
					build.thistype = build.Upgrade;
				}
			}
		}
	}

	if (Worldinformation::getinstance().mybuildings->getcompletecount(BWAPI::UnitTypes::Terran_Armory) > 0)
	{
		int mecheffected = getcountUnit(BWAPI::UnitTypes::Terran_Vulture, buildorder) + getcountUnit(BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode, buildorder) + getcountUnit(BWAPI::UnitTypes::Terran_Goliath, buildorder);

		if (!Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Vehicle_Plating) && Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Plating) != 3 && upgradecheck(BWAPI::UpgradeTypes::Terran_Vehicle_Plating, buildorder))
		{
			if (checkbuildandgame(buildorder, BWAPI::UpgradeTypes::Terran_Vehicle_Weapons, BWAPI::UnitTypes::Terran_Armory))
			{
				float armorscrore = clamp(linearandquadratic(0.0, mecheffected / getnumberscale(BWAPI::UpgradeTypes::Terran_Vehicle_Plating, 20), 0.6));
				armorscrore = scoringfixer(armorscrore, threatpunishscore);

				if (currentscore < armorscrore)
				{
					build.upgrade = BWAPI::UpgradeTypes::Terran_Vehicle_Plating;
					build.addonname = "";
					currentscore = armorscrore;
					build.thistype = build.Upgrade;
				}
			}
		}

		if (!Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons) && Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons) != 3 && upgradecheck(BWAPI::UpgradeTypes::Terran_Vehicle_Plating, buildorder))
		{
			if (checkbuildandgame(buildorder, BWAPI::UpgradeTypes::Terran_Vehicle_Plating, BWAPI::UnitTypes::Terran_Armory))
			{
				float attackscore = clamp(linearandquadratic(0.0, mecheffected / getnumberscale(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons,20), 0.8));
				attackscore = scoringfixer(attackscore, threatpunishscore);

				if (currentscore < attackscore)
				{
					build.upgrade = BWAPI::UpgradeTypes::Terran_Vehicle_Weapons;
					build.addonname = "";
					currentscore = attackscore;
					build.thistype = build.Upgrade;
				}
			}
		}


		int shipseffected = getcountUnit(BWAPI::UnitTypes::Terran_Wraith, buildorder) + getcountUnit(BWAPI::UnitTypes::Terran_Vulture, buildorder) + getcountUnit(BWAPI::UnitTypes::Terran_Battlecruiser, buildorder);
	}

	
	return build;
}

void upgradescoring::resetfromthreat(BWAPI::UnitType type)
{




}

float upgradescoring::getnumberscale(BWAPI::UpgradeType myupgrade, int defalutaddon)
{
	int addon = Broodwar->self()->getUpgradeLevel(myupgrade) * 5;

	return defalutaddon + addon;

}

bool upgradescoring::upgradecheck(BWAPI::UpgradeType myupgrade, std::vector<buildinfo> buildorder)
{
	if ((Broodwar->self()->getUpgradeLevel(myupgrade) > 0 && getcountbuilding(BWAPI::UnitTypes::Terran_Science_Facility, buildorder) > 1) || Broodwar->self()->getUpgradeLevel(myupgrade) == 0)
	{
		return true;
	}


	return false;
}

bool upgradescoring::checkbuildandgame(std::vector<buildinfo> buildorder, BWAPI::UpgradeType myupgrade, BWAPI::UnitType mybuilder)
{
	for (auto build : buildorder)
	{
		if (build.mytype == myupgrade)
		{
			return false;
		}
	}

	if (!((Broodwar->self()->isUpgrading(myupgrade) && Worldinformation::getinstance().mybuildings->getcompletecount(mybuilder) > 1) || !Broodwar->self()->isUpgrading(myupgrade)))
	{
		return false;
	}

	return true;

}