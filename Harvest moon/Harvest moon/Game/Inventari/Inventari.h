#pragma once
#include "Eines\Eina.h"

class Inventari {

public:
	void AgafarEina();
	void DeixarEina();
	void IntercanviarEina();

	int getDiners() { return _diners; }
	int getEinaActiva() { return _einaActiva; }
	bool getUtilitzarEina() { return _utilitzarEina; }


private:
	int _diners;
	int _einaActiva;
	bool _utilitzarEina;
	Eines _einesEquipades[2];
};

