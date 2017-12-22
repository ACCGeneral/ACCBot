#include "Counterlist.h"
using namespace BWAPI;

counterlist::counterlist()
{
	defaultscore = true;
	totalof3 = 0;

	counterdata Marine(BWAPI::UnitTypes::Terran_Marine);
	std::vector <BWAPI::UnitType> counter = { BWAPI::UnitTypes::Terran_SCV, BWAPI::UnitTypes::Terran_Dropship, BWAPI::UnitTypes::Terran_Science_Vessel, BWAPI::UnitTypes::Terran_Valkyrie, BWAPI::UnitTypes::Protoss_Corsair, BWAPI::UnitTypes::Protoss_Shuttle, BWAPI::UnitTypes::Protoss_Probe, BWAPI::UnitTypes::Zerg_Drone, BWAPI::UnitTypes::Zerg_Spore_Colony, BWAPI::UnitTypes::Zerg_Devourer, BWAPI::UnitTypes::Zerg_Scourge, BWAPI::UnitTypes::Terran_Goliath, BWAPI::UnitTypes::Terran_Wraith };
	std::vector <BWAPI::UnitType> counteredby = { BWAPI::UnitTypes::Terran_Battlecruiser, BWAPI::UnitTypes::Terran_Vulture, BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode, BWAPI::UnitTypes::Protoss_Archon, BWAPI::UnitTypes::Protoss_Carrier, BWAPI::UnitTypes::Protoss_Reaver, BWAPI::UnitTypes::Zerg_Guardian, BWAPI::UnitTypes::Zerg_Lurker, BWAPI::UnitTypes::Zerg_Ultralisk};
	Marine.sethardandsoft(counter, counteredby);
	units.push_back(Marine);

	counterdata firebat(BWAPI::UnitTypes::Terran_Firebat);
	counter = { BWAPI::UnitTypes::Zerg_Zergling, BWAPI::UnitTypes::Protoss_Zealot, BWAPI::UnitTypes::Zerg_Hydralisk, BWAPI::UnitTypes::Terran_Marine};
	counteredby = { BWAPI::UnitTypes::Terran_Vulture, BWAPI::UnitTypes::Terran_Wraith, BWAPI::UnitTypes::Terran_Battlecruiser, BWAPI::UnitTypes::Protoss_Dragoon, BWAPI::UnitTypes::Protoss_Reaver, BWAPI::UnitTypes::Protoss_Carrier, BWAPI::UnitTypes::Protoss_Scout, BWAPI::UnitTypes::Zerg_Lurker, BWAPI::UnitTypes::Zerg_Mutalisk, BWAPI::UnitTypes::Zerg_Guardian};
	firebat.sethardandsoft(counter, counteredby);
	units.push_back(firebat);

	counterdata ghosts(BWAPI::UnitTypes::Terran_Ghost);
	counter = {};
	counteredby = {};
	ghosts.sethardandsoft(counter, counteredby);
	units.push_back(ghosts);

	counterdata vulture(BWAPI::UnitTypes::Terran_Vulture);
	counter = {BWAPI::UnitTypes::Protoss_Archon, BWAPI::UnitTypes::Protoss_High_Templar, BWAPI::UnitTypes::Protoss_Zealot, BWAPI::UnitTypes::Terran_Firebat, BWAPI::UnitTypes::Terran_Marine};
	counteredby = { BWAPI::UnitTypes::Terran_Wraith, BWAPI::UnitTypes::Terran_Battlecruiser, BWAPI::UnitTypes::Terran_Goliath, BWAPI::UnitTypes::Zerg_Guardian, BWAPI::UnitTypes::Zerg_Mutalisk, BWAPI::UnitTypes::Protoss_Dragoon, BWAPI::UnitTypes::Protoss_Carrier, BWAPI::UnitTypes::Protoss_Scout};
	vulture.sethardandsoft(counter, counteredby);
	units.push_back(vulture);

	counterdata siegetank(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode);
	counter = {BWAPI::UnitTypes::Protoss_Dragoon, BWAPI::UnitTypes::Zerg_Infested_Terran, BWAPI::UnitTypes::Zerg_Hydralisk, BWAPI::UnitTypes::Zerg_Ultralisk, BWAPI::UnitTypes::Zerg_Lurker, BWAPI::UnitTypes::Terran_Marine, BWAPI::UnitTypes::Terran_Goliath};
	counteredby = { BWAPI::UnitTypes::Terran_Battlecruiser, BWAPI::UnitTypes::Protoss_Carrier, BWAPI::UnitTypes::Zerg_Mutalisk, BWAPI::UnitTypes::Terran_Wraith, BWAPI::UnitTypes::Terran_Ghost, BWAPI::UnitTypes::Protoss_Dark_Templar, BWAPI::UnitTypes::Protoss_Scout};
	siegetank.sethardandsoft(counter, counteredby);
	units.push_back(siegetank);

	counterdata Goliath(BWAPI::UnitTypes::Terran_Goliath);
	counter = {BWAPI::UnitTypes::Protoss_Scout, BWAPI::UnitTypes::Protoss_Corsair, BWAPI::UnitTypes::Protoss_Carrier, BWAPI::UnitTypes::Protoss_Arbiter, BWAPI::UnitTypes::Zerg_Mutalisk, BWAPI::UnitTypes::Zerg_Devourer, BWAPI::UnitTypes::Terran_Goliath, BWAPI::UnitTypes::Zerg_Scourge, BWAPI::UnitTypes::Terran_Firebat, BWAPI::UnitTypes::Terran_Vulture, BWAPI::UnitTypes::Terran_Wraith, BWAPI::UnitTypes::Terran_Valkyrie};
	counteredby = { BWAPI::UnitTypes::Terran_Marine, BWAPI::UnitTypes::Zerg_Zergling, BWAPI::UnitTypes::Zerg_Hydralisk, BWAPI::UnitTypes::Protoss_Zealot, BWAPI::UnitTypes::Protoss_Reaver, BWAPI::UnitTypes::Protoss_Dark_Templar, BWAPI::UnitTypes::Zerg_Lurker};
	Goliath.sethardandsoft(counter, counteredby);
	units.push_back(Goliath);

	counterdata wraith(BWAPI::UnitTypes::Terran_Wraith);
	counter = {BWAPI::UnitTypes::Terran_Vulture, BWAPI::UnitTypes::Terran_Wraith, BWAPI::UnitTypes::Zerg_Guardian, BWAPI::UnitTypes::Zerg_Scourge};
	counteredby = {BWAPI::UnitTypes::Terran_Valkyrie, BWAPI::UnitTypes::Zerg_Devourer, BWAPI::UnitTypes::Terran_Goliath, BWAPI::UnitTypes::Zerg_Hydralisk, BWAPI::UnitTypes::Protoss_Scout, BWAPI::UnitTypes::Protoss_Dragoon};
	wraith.sethardandsoft(counter, counteredby);
	units.push_back(wraith);

	counterdata battlecruiser(BWAPI::UnitTypes::Terran_Battlecruiser);
	counter =  {BWAPI::UnitTypes::Protoss_High_Templar, BWAPI::UnitTypes::Protoss_Reaver, BWAPI::UnitTypes::Protoss_Corsair, BWAPI::UnitTypes::Zerg_Zergling, BWAPI::UnitTypes::Terran_Ghost, BWAPI::UnitTypes::Terran_Vulture};
	counteredby = {BWAPI::UnitTypes::Protoss_Arbiter, BWAPI::UnitTypes::Protoss_Carrier, BWAPI::UnitTypes::Zerg_Defiler, BWAPI::UnitTypes::Terran_Goliath};
	battlecruiser.sethardandsoft(counter, counteredby);
	units.push_back(battlecruiser);

	counterdata Valkyrie(BWAPI::UnitTypes::Terran_Valkyrie);
	counter = {BWAPI::UnitTypes::Protoss_Scout, BWAPI::UnitTypes::Protoss_Corsair, BWAPI::UnitTypes::Zerg_Mutalisk, BWAPI::UnitTypes::Zerg_Guardian, BWAPI::UnitTypes::Zerg_Scourge, BWAPI::UnitTypes::Terran_Wraith};
	counteredby = { BWAPI::UnitTypes::Terran_Goliath, BWAPI::UnitTypes::Terran_Marine, BWAPI::UnitTypes::Zerg_Hydralisk, BWAPI::UnitTypes::Zerg_Scourge, BWAPI::UnitTypes::Protoss_Scout, BWAPI::UnitTypes::Protoss_Carrier, BWAPI::UnitTypes::Protoss_Dragoon};
	Valkyrie.sethardandsoft(counter, counteredby);
	units.push_back(Valkyrie);

	counterdata science(BWAPI::UnitTypes::Terran_Science_Vessel);
	counter = {BWAPI::UnitTypes::Zerg_Lurker, BWAPI::UnitTypes::Terran_Ghost, BWAPI::UnitTypes::Terran_Wraith, BWAPI::UnitTypes::Protoss_Dark_Templar};
	counteredby = { BWAPI::UnitTypes::Terran_Marine, BWAPI::UnitTypes::Zerg_Scourge, BWAPI::UnitTypes::Protoss_Scout, BWAPI::UnitTypes::Terran_Valkyrie, BWAPI::UnitTypes::Protoss_Dragoon, BWAPI::UnitTypes::Zerg_Hydralisk};
	science.sethardandsoft(counter, counteredby);
	units.push_back(science);

	updatadata();
}

float counterlist::getscore(BWAPI::UnitType unit)
{
	if (numofenemyunits <= 0)
	{
		return 0.7f;
	}

	for (int i = 0; i < units.size(); i++)
	{
		if (unit == units[i].returntype())
		{
			return units[i].returnscore();
		}
	}

}

void counterlist::addunit(BWAPI::UnitType unit)
{
	for (int i = 0; i < units.size(); i++)
	{
		units[i].checkscore(unit);
	}

	numofenemyunits++;
	
	defaultscore = false;

	updatadata();
}

void counterlist::removeunit(BWAPI::UnitType unit)
{
	for (int i = 0; i < units.size(); i++)
	{
		units[i].removescore(unit);
	}

	numofenemyunits--;
	

	updatadata();
}


float counterlist::percantageoftop3(BWAPI::UnitType buildingproducer)
{
	if (numofenemyunits <= 0)
	{
		return 0.7f;
	}

	BWAPI::UnitType barrack = BWAPI::UnitTypes::Terran_Barracks;
	BWAPI::UnitType factory = BWAPI::UnitTypes::Terran_Factory;
	BWAPI::UnitType starport = BWAPI::UnitTypes::Terran_Starport;

	float barrackunitscore = 0;
	float factoryunitscore = 0;
	float starportscore = 0;

	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].returntype().whatBuilds().first == barrack)
		{
			barrackunitscore += units[i].returnscore();
		}
		else if (units[i].returntype().whatBuilds().first == factory)
		{
			factoryunitscore += units[i].returnscore();
		}
		else if (units[i].returntype().whatBuilds().first == starport)
		{
			starportscore += units[i].returnscore();
		}
	}

	barrackunitscore /= 3.0f;
	factoryunitscore /= 3.0f;
	starportscore /= 4.0f;

	float maxscore = std::fmax(std::fmax(barrackunitscore, factoryunitscore),starportscore);

	if (buildingproducer == barrack)
	{
		return barrackunitscore / maxscore;
	}
	else if (buildingproducer == factory)
	{
		return factoryunitscore / maxscore;
	}
	else if (buildingproducer == starportscore)
	{
		return starportscore / maxscore;
	}
}

void counterlist::updatadata()
{
	totalof3 = 0;

	if (numofenemyunits == 0)
	{
		highestscore = 0.7;
		return;
	}

	top3.clear();
	highestscore = -100000;
	std::list<std::pair<int, BWAPI::UnitType>> scores;

	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].returnscore() > highestscore)
		{
			highestscore = units[i].returnscore();
		}

		std::pair<int, BWAPI::UnitType> newscore;
		newscore.first = units[i].returnscore();
		newscore.second = units[i].returntype();

		scores.push_back(newscore);
	}
	scores.sort();

	for (auto it = scores.rbegin(); it != scores.rend(); ++it)
	{
		if (top3.size() < 3)
		{
			top3.push_back(*it);
			totalof3 += it->first;
		}
	}


}