#include "DirectorEscenes.h"

#include "Especials/Intro.h"
#include "Especials/MainMenu.h"
#include "Granja/Granja.h"
#include "Granja/Casa.h"
#include "Granja/EstableGallines.h"
#include "Granja/EstableVaques.h"
#include "Granja/CasetaEines.h"
#include "Poble\Poble.h"

DirectorEscenes* DirectorEscenes::_directorEscenesInstance = NULL;


void DirectorEscenes::Update()
{
	EscenaEnum numeroEscena = _LASTSCENE;

	switch (_escenaActual)
	{
	case INTRO:				_vectorEscenes[INTRO]->Update();			break;
	case MAINMENU:			_vectorEscenes[MAINMENU]->Update();			break;
	case GRANJA:			_vectorEscenes[GRANJA]->Update();			numeroEscena = ((Granja*)_vectorEscenes[GRANJA])->getNumEscena();	break;
	case POBLE:				_vectorEscenes[POBLE]->Update();			break;
	case CASA:				_vectorEscenes[CASA]->Update();				break;
	case ESTABLE_GALLINES:	_vectorEscenes[ESTABLE_GALLINES]->Update();	break;
	case ESTABLE_VAQUES:	_vectorEscenes[ESTABLE_VAQUES]->Update();	break;
	case CASETA:			_vectorEscenes[CASETA]->Update();			break;
	case _LASTSCENE: break;
	default:
		break;
	}

	if (numeroEscena != _LASTSCENE)
		CanviEscena(numeroEscena);
}

void DirectorEscenes::Render()
{
	switch (_escenaActual)
	{
	case INTRO:				_vectorEscenes[INTRO]->Render();			break;
	case MAINMENU:			_vectorEscenes[MAINMENU]->Render();			break;
	case GRANJA:			_vectorEscenes[GRANJA]->Render();			break;
	case POBLE:				_vectorEscenes[POBLE]->Render();			break;
	case CASA:				_vectorEscenes[CASA]->Render();				break;
	case ESTABLE_GALLINES:	_vectorEscenes[ESTABLE_GALLINES]->Render();	break;
	case ESTABLE_VAQUES:	_vectorEscenes[ESTABLE_VAQUES]->Render();	break;
	case CASETA:			_vectorEscenes[CASETA]->Render();			break;
	case _LASTSCENE: break;
	default:
		break;
	}
}

void DirectorEscenes::CanviEscena(EscenaEnum SeguentEscena)
{
	_vectorEscenes[SeguentEscena]->ReInit();
	_escenaActual = SeguentEscena;

	NomEscenaActual();
}

void DirectorEscenes::NomEscenaActual()
{
	switch (_escenaActual)
	{
	case INTRO:				cout << "INTRO" << endl;			break;
	case MAINMENU:			cout << "MAINMENU" << endl;			break;
	case GRANJA:			cout << "GRANJA" << endl;			break;
	case POBLE:				cout << "POBLE" << endl;			break;
	case CASA:				cout << "CASA" << endl;				break;
	case ESTABLE_GALLINES:	cout << "ESTABLE_GALLINES" << endl; break;
	case ESTABLE_VAQUES:	cout << "ESTABLE_VAQUES" << endl;	break;
	case CASETA:			cout << "CASETA" << endl;			break;
	case _LASTSCENE:		cout << "_LASTSCENE" << endl;		break;
	default: break;
	}
}

DirectorEscenes::~DirectorEscenes()
{
}

DirectorEscenes::DirectorEscenes(Video* video, ResourceManager* rscManager, Jugador* jugador)
{
	_vectorEscenes.resize(NUM_SCENES);

	Intro* intro = new Intro();
	MainMenu* mainMenu = new MainMenu();
	Granja* granja = new Granja();
	Poble* poble = new Poble();
	Casa* casa = new Casa();
	EstableGallines* estableGallines = new EstableGallines();
	EstableVaques* estableVaques = new EstableVaques();
	CasetaEines* caseta = new CasetaEines();

	_vectorEscenes[INTRO] = intro;
	_vectorEscenes[MAINMENU] = mainMenu;
	_vectorEscenes[GRANJA] = granja;
	_vectorEscenes[POBLE] = poble;
	_vectorEscenes[CASA] = casa;
	_vectorEscenes[ESTABLE_GALLINES] = estableGallines;
	_vectorEscenes[ESTABLE_VAQUES] = estableVaques;
	_vectorEscenes[CASETA] = caseta;

	intro->Init(video, rscManager);
	mainMenu->Init(video, rscManager);
	granja->Init(video, rscManager);
	granja->setJugador(jugador);
	poble->Init(video, rscManager);
	casa->Init(video, rscManager);
	estableGallines->Init(video, rscManager);
	estableVaques->Init(video, rscManager);

	_escenaActual = GRANJA;
}