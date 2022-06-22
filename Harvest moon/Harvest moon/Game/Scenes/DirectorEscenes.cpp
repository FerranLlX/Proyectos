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
	case FARM:			_vectorEscenes[FARM]->Update();			numeroEscena = ((Granja*)_vectorEscenes[FARM])->getNumEscena();	break;
	case TOWN:				_vectorEscenes[TOWN]->Update();			break;
	case HOUSE:				_vectorEscenes[HOUSE]->Update();				break;
	case BARN_CHICKEN:	_vectorEscenes[BARN_CHICKEN]->Update();	break;
	case BARN_COW:	_vectorEscenes[BARN_COW]->Update();	break;
	case TOOL_SHED:			_vectorEscenes[TOOL_SHED]->Update();			break;
	case _LASTSCENE: break;
	default:
		break;
	}

	if (numeroEscena != _LASTSCENE)
		ChangeScene(numeroEscena);
}

void DirectorEscenes::Render()
{
	switch (_escenaActual)
	{
	case INTRO:				_vectorEscenes[INTRO]->Render();			break;
	case MAINMENU:			_vectorEscenes[MAINMENU]->Render();			break;
	case FARM:			_vectorEscenes[FARM]->Render();			break;
	case TOWN:				_vectorEscenes[TOWN]->Render();			break;
	case HOUSE:				_vectorEscenes[HOUSE]->Render();				break;
	case BARN_CHICKEN:	_vectorEscenes[BARN_CHICKEN]->Render();	break;
	case BARN_COW:	_vectorEscenes[BARN_COW]->Render();	break;
	case TOOL_SHED:			_vectorEscenes[TOOL_SHED]->Render();			break;
	case _LASTSCENE: break;
	default:
		break;
	}
}

void DirectorEscenes::ChangeScene(EscenaEnum SeguentEscena)
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
	case FARM:			cout << "GRANJA" << endl;			break;
	case TOWN:				cout << "POBLE" << endl;			break;
	case HOUSE:				cout << "CASA" << endl;				break;
	case BARN_CHICKEN:	cout << "ESTABLE_GALLINES" << endl; break;
	case BARN_COW:	cout << "ESTABLE_VAQUES" << endl;	break;
	case TOOL_SHED:			cout << "CASETA" << endl;			break;
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
	_vectorEscenes[FARM] = granja;
	_vectorEscenes[TOWN] = poble;
	_vectorEscenes[HOUSE] = casa;
	_vectorEscenes[BARN_CHICKEN] = estableGallines;
	_vectorEscenes[BARN_COW] = estableVaques;
	_vectorEscenes[TOOL_SHED] = caseta;

	intro->Init(video, rscManager);
	mainMenu->Init(video, rscManager);
	granja->Init(video, rscManager);
	granja->setJugador(jugador);
	poble->Init(video, rscManager);
	casa->Init(video, rscManager);
	estableGallines->Init(video, rscManager);
	estableVaques->Init(video, rscManager);

	_escenaActual = FARM;
}