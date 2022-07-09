#pragma once
#include <string>

using namespace std;

class Tool
{

public:
	string getName() { return _name; }
	int getIDRscManager() { return _idRscManager; }
	int getLevel() { return _level; }
	int getNumberOfUses() { return _numberOfUses; }


private:
	string _name;
	int _idRscManager;
	int _level;			// Nivell de millora
	int _numberOfUses;	// Galleda
};

