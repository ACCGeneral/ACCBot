#include "productionmanager.h"
using namespace BWAPI;

productionmanager::productionmanager()
{
	std::shared_ptr<UtilityFarm> unitscore = std::make_shared<unitscoring>(this);
	std::shared_ptr<UtilityFarm> builscorer = std::make_shared<buildingscore>(this);
	std::shared_ptr<UtilityFarm> addonscorer = std::make_shared<AddonScoring>(this);
	std::shared_ptr<UtilityFarm> upgradescore = std::make_shared<upgradescoring>(this);

	myfarms.push_back(unitscore);
	myfarms.push_back(builscorer);
	myfarms.push_back(addonscorer);
	myfarms.push_back(upgradescore);

	workers =  std::make_shared<workingmanager>();
	supplyset = 10.0f;
	bankedgas = 0;
	bankedminerals = 0;
	productionpower = 0;
	unittrainers = 0;
	buildordersupply = 0;

	myexpandbuilder.active = false;
	myexpandbuilder.builder = NULL;
}

void productionmanager::Addunit(BWAPI::Unit u)
{
	if (u->getType().isWorker() || u->getType().isRefinery() && u->getPlayer() == BWAPI::Broodwar->self())
	{
		workers->Addunit(u);
	}
	if ((u->getType().isBuilding() || u->getType().isAddon()) && u->getPlayer() == BWAPI::Broodwar->self())
	{
		bankedminerals -= u->getType().mineralPrice();
		bankedgas -= u->getType().gasPrice();

		for (int i = 0; i < buildingstomake.size(); i++)
		{
			if (buildingstomake[i].buildingtype == u->getType())
			{
				buildingstomake.erase(buildingstomake.begin() + i);
				break;
			}
		}

	}

	if (bankedgas < 0)
	{
		bankedgas = 0;
	}
	if (bankedminerals < 0)
	{
		bankedminerals = 0;
	}

}

void productionmanager::addcompleted(BWAPI::Unit u)
{
	if ((u->getType().isBuilding() || u->getType().isAddon()) && u->getPlayer() == BWAPI::Broodwar->self())
	{
		if (u->getType().isRefinery())
		{
			int i = 0;
		}

		buildings.insert(u);
	}

	if (u->getType().isResourceDepot() || u->getType().isWorker() || u->getType() == Broodwar->self()->getRace().getRefinery())
	{
		workers->addcompletedunit(u);
	}
}

void productionmanager::deleteunit(BWAPI::Unit u)
{
	if (u->getType().isWorker())
	{
		workers->deleteunit(u);

		if (u == myexpandbuilder.builder)
		{
			myexpandbuilder.builder = NULL;	
			myexpandbuilder.builder = workers->getworker();

			if (myexpandbuilder.builder == NULL)
			{
				myexpandbuilder.active = false;
				bankedminerals -= 400;
			}
			else
			{
				myexpandbuilder.expandtile = buildplacer.closestfreebase(myexpandbuilder.builder);
				myexpandbuilder.builder->move(Position(myexpandbuilder.expandtile.x, myexpandbuilder.expandtile.y));
			}
			
		}

		for (int i = 0; i < builders.size(); i++)
		{
			if (u == builders[i].Builder)
			{
				builders.erase(builders.begin() + i);
				return;
			}
		}

	}
	else if (u->getPlayer() == BWAPI::Broodwar->self())
	{
		if (u->getType().supplyProvided() > 0)
		{
			supplyset -= u->getType().supplyProvided()/2;
		}
		buildings.erase(u);
		builmanager.clearbuildorder();

		if (unitproducer(u->getType()))
		{
			unittrainers--;
		}
	}
}

bool productionmanager::checkresources(int minerals, int gas)
{
	if (Broodwar->self()->minerals() >= minerals + bankedminerals && Broodwar->self()->gas() >= gas + bankedgas)
	{
		return true;
	}
	return false;
}

void productionmanager::update()
{
	calculatePpower();

	for (int i = 0; i < myfarms.size(); i++)
	{
		myfarms[i]->update();
	}

	correctsupply = (Broodwar->self()->supplyUsed() + buildordersupply) / 2.0f;
	workers->update();
	whattobuild();
	build();
	buildersdone();
	emptybuildingcheck();

	threatresetcheck();
	

	if (myexpandbuilder.active == true)
	{
		if (buildplacer.expandbuild(myexpandbuilder.builder, myexpandbuilder.HQ, myexpandbuilder.expandtile))
		{
			builderdata newbuilderdata;
			newbuilderdata.Builder = myexpandbuilder.builder;
			newbuilderdata.startbuildtime = Broodwar->elapsedTime();
			builders.push_back(newbuilderdata);
			myexpandbuilder.active = false;
			myexpandbuilder.builder = NULL;
		}
		else if (myexpandbuilder.builder->isIdle())
		{
			myexpandbuilder.expandtile = buildplacer.closestfreebase(myexpandbuilder.builder);
			myexpandbuilder.builder->move(Position(myexpandbuilder.expandtile.x, myexpandbuilder.expandtile.y));
		}
	}

	int seconds = Broodwar->getFrameCount() / 24;
	for (int i = 0; i < buildingstomake.size(); i++)
	{
		if (seconds - buildingstomake[i].secondstart > buildingstomake[i].secondstowait)
		{
			if (buildingstomake[i].buildingtype.isAddon())
			{
				BWAPI::Unit addonbuilding = getaddonbuilding(buildingstomake[i].buildingtype);
				if (addonbuilding == NULL)
				{
					buildingstomake.erase(buildingstomake.begin() + i);
					i--;
				}
				else
				{
					if ((addonbuilding->isIdle() || !addonbuilding->isLifted()) || addonbuilding->getType().isResourceDepot())
					{
						buildplacer.placeAddon(addonbuilding, buildingstomake[i].buildingtype, buildingstomake[i].Positionstried);
					}
					buildingtimedata newset;
					newset.Positionstried = buildingstomake[i].Positionstried;
					newset = buildingstomake[i];
					newset.secondstart = seconds;
					newset.secondstowait = 5;
					buildingstomake.erase(buildingstomake.begin() + i);
					buildingstomake.push_back(newset);
					i--;
				}

			}
			else if (buildingstomake[i].buildingtype.isResourceDepot())
			{
				Unit builder = workers->getworker();
				myexpandbuilder.builder = builder;
				myexpandbuilder.HQ = buildingstomake[i].buildingtype;
				myexpandbuilder.expandtile = buildplacer.closestfreebase(builder);
				myexpandbuilder.builder->move(Position(myexpandbuilder.expandtile.x, myexpandbuilder.expandtile.y));
				myexpandbuilder.active = true;

				int seconds = Broodwar->getFrameCount() / 24;
				buildingtimedata newset;
				newset.secondstowait = 20;
				newset.secondstart = seconds;
				newset.buildingtype = buildingstomake[i].buildingtype;
				buildingstomake.push_back(newset);
				buildingstomake.erase(buildingstomake.begin() + i);
				i--;
			}
			else
			{
				BWAPI::Unit builder = workers->getworker();
				buildplacer.placeBuilding(builder, buildingstomake[i].buildingtype);
				builderdata newbuilderdata;
				newbuilderdata.Builder = builder;
				newbuilderdata.startbuildtime = Broodwar->elapsedTime();
				builders.push_back(newbuilderdata);
				buildingtimedata newset;
				newset = buildingstomake[i];
				newset.secondstart = seconds;
				newset.secondstowait = 10;
				buildingstomake.erase(buildingstomake.begin() + i);
				buildingstomake.push_back(newset);
				i--;
			}
		}
	}

}

std::set<BWAPI::Unit> productionmanager::returnworkers()
{
	return workers->returnAllW();
}

std::set<BWAPI::Unit> productionmanager::returnAWorkers()
{
	return workers->returnAvaliableW();
}

void productionmanager::build()
{
	std::vector<buildinfo> getbuild = builmanager.returntobuild();

	for (int i = 0; i < getbuild.size(); i++)
	{
		if (i == 0 || buildordercheck(getbuild[i].mineralcost, getbuild[i].gascost, getbuild[0].mineralcost, getbuild[0].gascost))
		{
			if (getbuild.size() > 0 && checkresources(getbuild[i].mineralcost, getbuild[i].gascost) && supplycheck(getbuild[i].mytype.supplyRequired()))
			{
				if (getbuild[i].whatbuildsme.isWorker() && !getbuild[i].mytype.isResourceDepot())
				{
					BWAPI::Unit builder = workers->getworker();
					if (builder != NULL)
					{
						if (buildplacer.placeBuilding(builder, getbuild[i].mytype))
						{
							bankedminerals += getbuild[i].mytype.mineralPrice();
							bankedgas += getbuild[i].mytype.gasPrice();
							builderdata newbuilderdata;
							newbuilderdata.Builder = builder;
							newbuilderdata.startbuildtime = Broodwar->elapsedTime();
							builders.push_back(newbuilderdata);
							Broodwar << "Buliding " << getbuild[i].mytype << std::endl;
							Broodwar << "Unit score was " << getbuild[i].buildscore << std::endl;

							for (int j = 0; j < myfarms.size(); j++)
							{
								myfarms[j]->setcooldown(getbuild[i].mytype);
							}

							int seconds = Broodwar->getFrameCount() / 24;
							buildingtimedata newset;
							newset.secondstowait = 10;
							newset.secondstart = seconds;
							newset.buildingtype = getbuild[i].mytype;
							buildingstomake.push_back(newset);

							builmanager.removenum(i);
							getbuild = builmanager.returntobuild();
							i--;
						}
					}
				}
				else if (getbuild[i].mytype.isResourceDepot() && myexpandbuilder.active == false)
				{
					Unit builder = workers->getworker();

					if (buildplacer.closestfreebase(builder) != BWAPI::TilePosition(-1, -1))
					{
						myexpandbuilder.active = true;
						myexpandbuilder.builder = builder;
						myexpandbuilder.HQ = getbuild[i].mytype;
						myexpandbuilder.expandtile = buildplacer.closestfreebase(builder);

						Broodwar << "Buliding " << getbuild[i].mytype << std::endl;
						Broodwar << "Unit score was " << getbuild[i].buildscore << std::endl;

						for (int j = 0; j < myfarms.size(); j++)
						{
							myfarms[j]->setcooldown(getbuild[i].mytype);
						}

						int seconds = Broodwar->getFrameCount() / 24;
						buildingtimedata newset;
						newset.secondstowait = 20;
						newset.secondstart = seconds;
						newset.buildingtype = getbuild[i].mytype;
						buildingstomake.push_back(newset);

						bankedminerals += getbuild[i].mytype.mineralPrice();
						bankedgas += getbuild[i].mytype.gasPrice();
						myexpandbuilder.builder->move(Position(myexpandbuilder.expandtile.x, myexpandbuilder.expandtile.y));
					}

					builmanager.removenum(i);
					getbuild = builmanager.returntobuild();
					i--;
				}
				else if (getbuild[i].mytype.isAddon())
				{
					Unit addonbuilding = getaddonbuilding(getbuild[i].mytype);

					if (addonbuilding == NULL)
					{
						builmanager.removenum(i);
						getbuild = builmanager.returntobuild();
						i--;
					}
					else if (addonbuilding != NULL && addonbuilding->getTrainingQueue().size() == 0)
					{
						buildingtimedata newset;
						if (buildplacer.placeAddon(addonbuilding, getbuild[i].mytype, newset.Positionstried))
						{
							Broodwar << "Buliding " << getbuild[i].mytype << std::endl;
							Broodwar << "Unit score was " << getbuild[i].buildscore << std::endl;
						}

						int seconds = Broodwar->getFrameCount() / 24;
						newset.secondstart = seconds;
						newset.buildingtype = getbuild[i].mytype;
						newset.secondstowait = 4;
						buildingstomake.push_back(newset);
						builmanager.removenum(i);
						getbuild = builmanager.returntobuild();
						i--;
					}
				}
				else if (getbuild[i].thistype == getbuild[i].Tech || getbuild[i].thistype == getbuild[i].Upgrade)
				{
					std::vector<Unit> buildingwewant;

					for (auto building : buildings)
					{
						if (getbuild[i].whatbuildsme == building->getType())
						{
							buildingwewant.push_back(building);
						}
					}

					Unit building = returnbuilding(buildingwewant, getbuild[i]);
					
					if (building != NULL && !building->isUpgrading() && !building->isConstructing())
					{
						if (getbuild[i].thistype == getbuild[i].Tech)
						{
							Broodwar << "researching " << getbuild[i].technology << std::endl;
							Broodwar << Broodwar->getLastError() << std::endl;
							building->research(getbuild[i].technology);
						}
						else
						{
							Broodwar << "researching " << getbuild[i].upgrade << std::endl;
							Broodwar << Broodwar->getLastError() << std::endl;
							building->upgrade(getbuild[i].upgrade);
						}

						Broodwar << "Unit score was " << getbuild[i].buildscore << std::endl;
						buildordersupply -= getbuild[i].mytype.supplyRequired();
						builmanager.removenum(i);
						getbuild = builmanager.returntobuild();
						i--;
					}
				}
				else
				{
					std::vector<Unit> buildingwewant;

					for (auto building : buildings)
					{
						if (getbuild[i].whatbuildsme == building->getType())
						{
							if (getbuild[i].addonname == "")
							{
								buildingwewant.push_back(building);
							}
							else if (building->getAddon() != NULL && getbuild[i].addonname != "")
							{	
								std::string ihave = building->getAddon()->getType().getName();
								if (ihave == getbuild[i].addonname)
								{
									buildingwewant.push_back(building);
								}
							}

						}
					}

					Unit building = returnbuilding(buildingwewant, getbuild[i]);

					if (building != NULL && building->getTrainingQueue().size() == 0 && !building->isConstructing())
					{
						Broodwar << "Training " << getbuild[i].mytype << std::endl;
						Broodwar << "Unit score was " << getbuild[i].buildscore << std::endl;

						building->train(getbuild[i].mytype);
						buildordersupply -= getbuild[i].mytype.supplyRequired();
						builmanager.removenum(i);
						getbuild = builmanager.returntobuild();
						i--;
					}
				}
			}
		}
	}
}


void productionmanager::whattobuild()
{
	buildinfo tobuild;
	float score = 0.000000f;

	if (builmanager.returntobuild().size() >= 3)
	{
		return;
	}

	for (int i = 0; i < myfarms.size(); i++)
	{
		tobuild = myfarms[i]->scoring(score, tobuild, builmanager.returntobuild());
	}

	if (score != 0)
	{
		if (tobuild.mytype.supplyProvided() != 0)
		{
			supplyset += tobuild.mytype.supplyProvided()/2;
		}

		tobuild.setup(score);
		if (tobuild.major == false)
		{
			builmanager.addtobuild(tobuild);
		}
		else
		{
			builmanager.addtofront(tobuild);
		}

		if (unitproducer(tobuild.mytype))
		{
			unittrainers++;
		}

		for (int i = 0; i < myfarms.size(); i++)
		{
			myfarms[i]->setcooldownbool(tobuild.mytype);
		}

		buildordersupply += tobuild.mytype.supplyRequired();

	}

}


void productionmanager::buildersdone()
{
	int currenttime = Broodwar->elapsedTime();

	for (int i = 0; i < builders.size(); i++)
	{
		if (builders[i].Builder == NULL)
		{
			builders.erase(builders.begin() + i);
			i--;
		}
		else if (builders[i].Builder->isIdle() || builders[i].Builder->isGatheringGas())
		{
			workers->addtofree(builders[i].Builder);
			builders.erase(builders.begin() + i);
			i--;
		}
		else if ((builders[i].startbuildtime + 150) <= currenttime) //timeout
		{
			builders[i].Builder->stop();
			workers->addtofree(builders[i].Builder);
			builders.erase(builders.begin() + i);
			i--;
		}

	}
}


void productionmanager::emptybuildingcheck()
{
	if (Broodwar->elapsedTime() % 15 == 0)
	{
		for (auto building : Broodwar->self()->getUnits())
		{
			if (building->getType().isBuilding())
			{
				if (!building->isBeingConstructed() && !building->isCompleted())
				{
					BWAPI::Unit builder = workers->getworker();
					if (builder != NULL)
					{
						builder->rightClick(building);
						builderdata newbuilderdata;
						newbuilderdata.Builder = builder;
						newbuilderdata.startbuildtime = Broodwar->elapsedTime();
						builders.push_back(newbuilderdata);
					}
				}
			}
		}
	}

}


bool productionmanager::supplycheck(int supply)
{
	if (Broodwar->self()->supplyUsed() + supply <= Broodwar->self()->supplyTotal())
	{
		return true;
	}

	return false;
}

void productionmanager::calculatePpower()
{
	productionpower = (((workers->getcurrentworkernum() + getfromproduction(builmanager.returntobuild(), BWAPI::UnitTypes::Terran_SCV)) / std::fmin(workers->basenum(),1)) / 21.0f) * 10.0f;
	maxtrainers = 5 * workers->basenum();
	float unitproducters = unittrainers;
	productionpower = productionpower * fabs((std::fmin(unitproducters / maxtrainers, 1.0f)) - 1);
	
}

bool productionmanager::unitproducer(BWAPI::UnitType type)
{
	if (type == BWAPI::UnitTypes::Terran_Barracks || type == BWAPI::UnitTypes::Terran_Factory || type == BWAPI::UnitTypes::Terran_Starport)
	{
		return true;
	}

	return false;
}

BWAPI::Unit productionmanager::returnbuilding(std::vector<BWAPI::Unit> possiblebuild, buildinfo myinfo)
{
	BWAPI::Unit building = NULL;

	if (myinfo.thistype == myinfo.Unit)
	{
		int queuesize = 10;
	
		for (int i = 0; i < possiblebuild.size(); i++)
		{
			if (possiblebuild[i]->getTrainingQueue().size() < queuesize && possiblebuild[i]->getTrainingQueue().size() != 5 && (!possiblebuild[i]->isFlying()))
			{
				building = possiblebuild[i];
				queuesize = possiblebuild[i]->getTrainingQueue().size();
			}
		}
	}
	else if (myinfo.thistype == myinfo.Tech)
	{
		for (int i = 0; i < possiblebuild.size(); i++)
		{
			if (!possiblebuild[i]->isResearching())
			{
				building = possiblebuild[i];
				break;
			}
		}
	}
	else if (myinfo.thistype == myinfo.Upgrade)
	{
		for (int i = 0; i < possiblebuild.size(); i++)
		{
			if (!possiblebuild[i]->isUpgrading())
			{
				building = possiblebuild[i];
				break;
			}
		}
	}


	return building;
}

bool productionmanager::buildordercheck(int minerals, int gas, int firstminerals, int firstgas)
{
	if (Broodwar->self()->minerals() >= minerals + firstminerals + bankedminerals && Broodwar->self()->gas() >= gas + firstgas + bankedgas)
	{
		return true;
	}

	return false;
}

int productionmanager::gettobebuiltcount(BWAPI::UnitType lookfor)
{
	int count = 0;

	for (int i = 0; i < buildingstomake.size(); i++)
	{
		if (buildingstomake[i].buildingtype == lookfor)
		{
			count++;
		}
	}

	return count;
}

bool productionmanager::addoncheck(BWAPI::UnitType addonbuilding)
{
	for (auto building : buildings)
	{
		if (building->getType() == addonbuilding && building->getAddon() == NULL)
		{
			return true;
		}
	}

	return false;
}

BWAPI::Unit productionmanager::getaddonbuilding(BWAPI::UnitType addonbuilding)
{
	for (auto building : buildings)
	{
		if (building->getType() == addonbuilding.whatBuilds().first && building->getAddon() == NULL)
		{
			return building;
		}
	}

	return NULL;
}

bool productionmanager::freequeue(BWAPI::UnitType unit)
{
	std::vector<Unit> buildingwewant;

	for (auto building : buildings)
	{
		if (unit.whatBuilds().first == building->getType())
		{
			if (building->getTrainingQueue().size() == 0)
			{
				return true;
			}
		}
	}

	return false;
}

void productionmanager::threatresetcheck()
{
	if (Worldinformation::getinstance().returnthreatspike())
	{
		for (auto type : builmanager.returntobuild())
		{
			if (type.mytype.supplyProvided() > 0)
			{
				supplyset -= type.mytype.supplyProvided() / 2;
			}

			for (int i = 0; i < myfarms.size(); i++)
			{
				myfarms[i]->resetfromthreat(type.mytype);
			}

		}
		builmanager.clearbuildorder();
		Worldinformation::getinstance().resetthreatspike();
	}


}