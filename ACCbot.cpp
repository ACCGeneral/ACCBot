#include "ACCbot.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;


void ACCbotmain::onStart()
{

	AICommander = new Commander();

	Broodwar->sendText("ACCBot v1.0");

	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	Broodwar->enableFlag(Flag::UserInput);

	Broodwar->setLocalSpeed(10);

	Broodwar->setCommandOptimizationLevel(2);

	if (Broodwar->isReplay())
	{
		Broodwar << "The following players are in this replay:" << std::endl;

		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
			if (!p->isObserver())
				Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
		}

	}
	else 
	{
			Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
	}

	BWTA::readMap();
	analyzed = false;
	analysis_just_finished = false;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);

}

void ACCbotmain::onEnd(bool isWinner)
{
	
	Worldinformation::getinstance().cleanup();
	influenceMapData::getinstance().cleanup();
	AICommander->cleanup();
	delete AICommander;

}

void ACCbotmain::onFrame()
{

	AICommander->update();

	// Called once every game frame
	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());


	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;


	if (analyzed)
	{
		drawTerrainData();
	}

	if (analysis_just_finished)
	{
		Broodwar << "Finished analyzing map." << std::endl;
		analysis_just_finished = false;
	}

	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;
}

void ACCbotmain::onSendText(std::string text)
{

	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());


	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!

	if (text == "/a")
	{
		if (analyzed == false)
		{
			Broodwar << "Analyzing map... this may take a minute" << std::endl;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
		}
	}
	else
	{
		Broodwar->sendText("%s", text.c_str());
	}

}

void ACCbotmain::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ACCbotmain::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ACCbotmain::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ACCbotmain::onUnitDiscover(BWAPI::Unit unit)
{

}

void ACCbotmain::onUnitEvade(BWAPI::Unit unit)
{

}


void ACCbotmain::onUnitShow(BWAPI::Unit unit)
{
	AICommander->showunit(unit);
}

void ACCbotmain::onUnitHide(BWAPI::Unit unit)
{
	AICommander->unithiding(unit);

}

void ACCbotmain::onUnitCreate(BWAPI::Unit unit)
{
	AICommander->createunit(unit);
	
}

void ACCbotmain::onUnitDestroy(BWAPI::Unit unit)
{
	AICommander->unitdead(unit);
}

void ACCbotmain::onUnitMorph(BWAPI::Unit unit)
{
	AICommander->unitmorph(unit);
}

void ACCbotmain::onUnitRenegade(BWAPI::Unit unit)
{
	AICommander->traitor(unit);
}

void ACCbotmain::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ACCbotmain::onUnitComplete(BWAPI::Unit unit)
{
	AICommander->unitcomplete(unit);
}


DWORD WINAPI AnalyzeThread()
{
	BWTA::analyze();
	analyzed = true;
	analysis_just_finished = true;
	influenceMapData::getinstance().setupworldinfo();
	return 0;
}


void ACCbotmain::drawTerrainData()
{
	//we will iterate through all the base locations, and draw their outlines.
	for (const auto& baseLocation : BWTA::getBaseLocations()) {
		TilePosition p = baseLocation->getTilePosition();

		//draw outline of center location
		Position leftTop(p.x * TILE_SIZE, p.y * TILE_SIZE);
		Position rightBottom(leftTop.x + 4 * TILE_SIZE, leftTop.y + 3 * TILE_SIZE);
		Broodwar->drawBoxMap(leftTop, rightBottom, Colors::Blue);

		//draw a circle at each mineral patch
		for (const auto& mineral : baseLocation->getStaticMinerals()) {
			Broodwar->drawCircleMap(mineral->getInitialPosition(), 30, Colors::Cyan);
		}

		//draw the outlines of Vespene geysers
		for (const auto& geyser : baseLocation->getGeysers()) {
			TilePosition p1 = geyser->getInitialTilePosition();
			Position leftTop1(p1.x * TILE_SIZE, p1.y * TILE_SIZE);
			Position rightBottom1(leftTop1.x + 4 * TILE_SIZE, leftTop1.y + 2 * TILE_SIZE);
			Broodwar->drawBoxMap(leftTop1, rightBottom1, Colors::Orange);
		}

		//if this is an island expansion, draw a yellow circle around the base location
		if (baseLocation->isIsland()) {
			Broodwar->drawCircleMap(baseLocation->getPosition(), 80, Colors::Yellow);
		}
	}

	//we will iterate through all the regions and ...
	for (const auto& region : BWTA::getRegions()) {
		// draw the polygon outline of it in green
		BWTA::Polygon p = region->getPolygon();
		for (size_t j = 0; j < p.size(); ++j) {
			Position point1 = p[j];
			Position point2 = p[(j + 1) % p.size()];
			Broodwar->drawLineMap(point1, point2, Colors::Green);
		}
		// visualize the chokepoints with red lines
		for (auto const& chokepoint : region->getChokepoints()) {
			Position point1 = chokepoint->getSides().first;
			Position point2 = chokepoint->getSides().second;
			Broodwar->drawLineMap(point1, point2, Colors::Red);
		}
	}
}


//Reference to the BWTA2 library for the terrian analysis as well as the drawTerrainData and AnalyzeThread code. Can be found at https://bitbucket.org/auriarte/bwta2/wiki/Getting%20Started