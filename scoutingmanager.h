#pragma once

#include "BWAPI.h"
#include "BWTA.h"
#include "Worldinformation.h"
#include "influenceMapData.h"
#include "productionmanager.h"

class scoutingmanager
{
private:

	BWAPI::Unit enemyscouter;
	BWAPI::Unit freescouter;
	std::vector<float> scoutenemybasetime;
	std::vector<float> scoutfreebasestime;
	productionmanager *pro;
	void scoutbaselocations(BWAPI::Unit finder);
	void findenemybase(BWAPI::Unit scouter);

	void finderlogic(BWAPI::Unit finder);
	void enemyscoutstate(BWAPI::Unit scouter);

	enum states {moving,movearound};
	states enmeyscoutstate;

	int currectbase;

public:

	scoutingmanager(productionmanager *p);
	void update();




};
