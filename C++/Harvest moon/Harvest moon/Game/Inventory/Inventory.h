#pragma once
#include "Tools\TypesTools.h"

class Inventory {

public:
	void Tool_PickUp();
	void Tool_Leave();
	void Tool_Interchange();

	int getMoney() { return _money; }
	int getActiveTool() { return _activeTool; }
	bool getCanUseTool() { return _canUseTool; }


private:
	int _money;
	int _activeTool;
	bool _canUseTool;
	TypesTools _equippedTool[2];
};

