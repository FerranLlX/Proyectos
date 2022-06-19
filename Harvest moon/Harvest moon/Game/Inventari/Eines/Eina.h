#pragma once
#include <string>

using namespace std;

enum Eines
{
	MARTELL, AIXADA, FALÇ, DESTRAL
};


class Eina
{

public:
	string getNom() { return _nom; }
	int getIDRscManager() { return _idRscManager; }
	int getNivell() { return _nivell; }
	int getNombreDusos() { return _nombreDusos; }


private:
	string _nom;
	int _idRscManager;
	int _nivell;			// Nivell de millora
	int _nombreDusos;	// Galleda
};

