#pragma once

#include "BWAPI.h"
#include "productionmanager.h"
#include "scoutingmanager.h"
#include "unitmanager.h"
#include "Worldinformation.h"
#include "influenceMapData.h"


//Design of My bot design is based on a implementation of the Bot "UAlbertaBot" created by David Churchill https://github.com/davechurchill/ualbertabot/wiki

class Commander
{
private:

	productionmanager *myproductmanager;
	scoutingmanager *scoutman;
	unitmanager *unitcomander;


public:
	Commander();
	~Commander();

	void update();

	void createunit(BWAPI::Unit unit);
	void unitdead(BWAPI::Unit unit);
	void unitmorph(BWAPI::Unit unit);
	void unitcomplete(BWAPI::Unit unit);
	void showunit(BWAPI::Unit unit);
	void unithiding(BWAPI::Unit unit);
	void traitor(BWAPI::Unit unit);

	void cleanup();
};


