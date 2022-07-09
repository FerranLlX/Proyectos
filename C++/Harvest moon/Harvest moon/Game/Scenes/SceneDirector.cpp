#include "SceneDirector.h"

#include "Special\Intro.h"
#include "Special\MainMenu.h"
#include "Farm\Farm.h"
#include "Farm\House.h"
#include "Farm\BarnChicken.h"
#include "Farm\BarnCow.h"
#include "Farm\ToolHouse.h"
#include "Town\Town.h"

SceneDirector* SceneDirector::_directorScenesInstance = NULL;


void SceneDirector::Update()
{
	EscenEnum numScene = _LASTSCENE;

	switch (_actualScene)
	{
	case INTRO:			_vectorScenes[INTRO]->Update();			break;
	case MAINMENU:		_vectorScenes[MAINMENU]->Update();		break;
	case FARM:			_vectorScenes[FARM]->Update();			numScene = ((Farm*)_vectorScenes[FARM])->getNumScene();	break;
	case TOWN:			_vectorScenes[TOWN]->Update();			break;
	case HOUSE:			_vectorScenes[HOUSE]->Update();			break;
	case BARN_CHICKEN:	_vectorScenes[BARN_CHICKEN]->Update();	break;
	case BARN_COW:		_vectorScenes[BARN_COW]->Update();		break;
	case TOOL_SHED:		_vectorScenes[TOOL_SHED]->Update();		break;
	case _LASTSCENE: break;
	default:
		break;
	}

	if (numScene != _LASTSCENE)
		ChangeScene(numScene);
}

void SceneDirector::Render()
{
	switch (_actualScene)
	{
	case INTRO:				_vectorScenes[INTRO]->Render();			break;
	case MAINMENU:			_vectorScenes[MAINMENU]->Render();		break;
	case FARM:				_vectorScenes[FARM]->Render();			break;
	case TOWN:				_vectorScenes[TOWN]->Render();			break;
	case HOUSE:				_vectorScenes[HOUSE]->Render();			break;
	case BARN_CHICKEN:		_vectorScenes[BARN_CHICKEN]->Render();	break;
	case BARN_COW:			_vectorScenes[BARN_COW]->Render();		break;
	case TOOL_SHED:			_vectorScenes[TOOL_SHED]->Render();		break;
	case _LASTSCENE: break;
	default:
		break;
	}
}

void SceneDirector::ChangeScene(EscenEnum NextScene)
{
	_vectorScenes[NextScene]->ReInit();
	_actualScene = NextScene;

	NameActualScene();
}

void SceneDirector::NameActualScene()
{
	switch (_actualScene)
	{
	case INTRO:				cout << "INTRO" << endl;			break;
	case MAINMENU:			cout << "MAINMENU" << endl;			break;
	case FARM:				cout << "FARM" << endl;				break;
	case TOWN:				cout << "TOWN" << endl;				break;
	case HOUSE:				cout << "HOUSE" << endl;			break;
	case BARN_CHICKEN:		cout << "BARN_CHIKEN" << endl;		break;
	case BARN_COW:			cout << "BARN_COW" << endl;			break;
	case TOOL_SHED:			cout << "TOOL_HOUSE" << endl;		break;
	case _LASTSCENE:		cout << "_LASTSCENE" << endl;		break;
	default: break;
	}
}

SceneDirector::~SceneDirector()
{
}

SceneDirector::SceneDirector(Video* video, ResourceManager* rscManager, Player* jugador)
{
	_vectorScenes.resize(NUM_SCENES);

	Intro* intro = new Intro();
	MainMenu* mainMenu = new MainMenu();
	Farm* farm = new Farm();
	Town* town = new Town();
	House* house = new House();
	BarnChicken* barnChicken = new BarnChicken();
	BarnCow* barnCow = new BarnCow();
	ToolHouse* toolHouse = new ToolHouse();

	_vectorScenes[INTRO] = intro;
	_vectorScenes[MAINMENU] = mainMenu;
	_vectorScenes[FARM] = farm;
	_vectorScenes[TOWN] = town;
	_vectorScenes[HOUSE] = house;
	_vectorScenes[BARN_CHICKEN] = barnChicken;
	_vectorScenes[BARN_COW] = barnCow;
	_vectorScenes[TOOL_SHED] = toolHouse;

	intro->Init(video, rscManager);
	mainMenu->Init(video, rscManager);
	farm->Init(video, rscManager);
	farm->setPlayer(jugador);
	town->Init(video, rscManager);
	house->Init(video, rscManager);
	barnChicken->Init(video, rscManager);
	barnCow->Init(video, rscManager);

	_actualScene = FARM;
}