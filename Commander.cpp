#include "Commander.h"
using namespace BWAPI;

Commander::Commander()
{
	myproductmanager = new productionmanager();
	scoutman = new scoutingmanager(myproductmanager);
	unitcomander = new unitmanager();

}

Commander::~Commander()
{



}

void Commander::update()
{

	Worldinformation::getinstance().update();

	myproductmanager->update();

	scoutman->update();

	unitcomander->update();

}

void Commander::createunit(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding() || unit->getType().isWorker() || unit->getType().isAddon())
	{
		myproductmanager->Addunit(unit);
	}

	Worldinformation::getinstance().addunit(unit);
}

void Commander::unitdead(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding() || unit->getType().isWorker() || unit->getType().isAddon())
	{
		myproductmanager->deleteunit(unit);
	}
	else if (!unit->getType().isBuilding() && unit->getPlayer() == Broodwar->self())
	{
		unitcomander->ondeath(unit);
	}


	Worldinformation::getinstance().removeunit(unit);
	influenceMapData::getinstance().removeenemyarea(unit);
	influenceMapData::getinstance().removemybase(unit);
}

void Commander::unitmorph(BWAPI::Unit unit)
{
	if (unit->getType().isBuilding() || unit->getType().isWorker())
	{
		myproductmanager->Addunit(unit);
	}

	if (!unit->getType().isBuilding())
	{
		unitcomander->addunit(unit);
		Worldinformation::getinstance().onmorph(unit);
	}


}

void Commander::unitcomplete(BWAPI::Unit unit)
{
	unitcomander->addunit(unit);
	
	if (unit->getType().isBuilding() || unit->getType().isWorker() || unit->getType().isAddon())
	{
		myproductmanager->addcompleted(unit);
	}

	if ((unit->getType().isBuilding() || unit->getType().isAddon()) && unit->getPlayer() == Broodwar->self())
	{
		Worldinformation::getinstance().mybuildings->addtocompleted(unit);
	}

}

void Commander::showunit(BWAPI::Unit unit)
{
	unitcomander->addenemyunit(unit);
	Worldinformation::getinstance().onshow(unit);
	influenceMapData::getinstance().addenemyarea(unit);
	influenceMapData::getinstance().addmybase(unit);
	
}

void Commander::unithiding(BWAPI::Unit unit)
{
	unitcomander->removeenemyunit(unit);

}

void Commander::traitor(BWAPI::Unit unit)
{


}

void Commander::cleanup()
{
	delete myproductmanager;
	delete unitcomander;
	delete scoutman;

}



