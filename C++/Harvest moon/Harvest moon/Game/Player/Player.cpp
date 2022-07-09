#include "Player.h"

Player::Player(Video* video, ResourceManager* rscManager)
{
	_video = video;
	_rscManager = rscManager;

	_posPlayerX = 0;
	_posPlayerY = 0;

	_actualDirection = SOUTH;
	_actualAnimation = IDLE;
	_numFrameAnimation = 0;

	_actualDraw.h = 0;
	_actualDraw.w = 0;
	_actualDraw.x = 0;
	_actualDraw.y = 0;

	LoadJugador();
}

void Player::RenderPlayer(int posX, int posY)
{
	Animation_Walk(_actualDirection);

	Xywh desti{};
	desti.x = posX;
	desti.y = posY;
	desti.w = _actualDraw.w;
	desti.h = _actualDraw.h;

	_video->renderGraphic(_idRscMngPlayer, &_actualDraw, &desti);
}

void Player::UpdatePlayer()
{
}



#pragma region Animacions

void Player::Animation_Walk(Direction _actualDirection)
{
	switch (_actualDirection)
	{
	case NORTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = 32 * 3;
		break;
	case SOUTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = 32 * 0;
		break;
	case WEST:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = 32 * 2;
		break;
	case EAST:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = 32 * 1;
		break;
	default:
		break;
	}

	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_Run(Direction _actualDirection)
{
	switch (_actualDirection)
	{
	case NORTH:
		_actualDraw.x = (_numFrameAnimation * 16) + (16 * 2);
		_actualDraw.y = 32 * 3;
		break;
	case SOUTH:
		_actualDraw.x = (_numFrameAnimation * 16) + (16 * 2);
		_actualDraw.y = 32 * 0;
		break;
	case WEST:
		_actualDraw.x = (_numFrameAnimation * 16) + (16 * 2);
		_actualDraw.y = 32 * 2;
		break;
	case EAST:
		_actualDraw.x = (_numFrameAnimation * 16) + (16 * 2);
		_actualDraw.y = 32 * 1;
		break;
	default:
		break;
	}

	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_Harvest(Direction _actualDirection)
{
	switch (_actualDirection)
	{
	case NORTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 3) + 112;
		break;
	case SOUTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 0) + 112;
		break;
	case WEST:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 2) + 112;
		break;
	case EAST:
		// PENDENT: FER FLIP
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 1) + 112;
		break;
	default:
		break;
	}

	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_WipeSweat()
{
	// Posicio inicial
	_actualDraw.x = 0;
	_actualDraw.y = 224;
	// Mida
	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_Tired()
{
	// Posicio inicial
	_actualDraw.x = 0;
	_actualDraw.y = 288;
	// Mida
	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_WornOut()
{
	// Posicio inicial
	_actualDraw.x = 0;
	_actualDraw.y = 336;
	// Mida
	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_Drink()
{
	// Posicio inicial
	_actualDraw.x = 0;
	_actualDraw.y = 384;
	// Mida
	_actualDraw.w = 22;
	_actualDraw.h = 32;
}

void Player::Animation_Jump(Direction _actualDirection)
{
	switch (_actualDirection)
	{
	case NORTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 3) + 512;
		break;
	case SOUTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 0) + 512;
		break;
	case WEST:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 2) + 512;
		break;
	case EAST:
		// PENDENT: FER FLIP
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 1) + 512;
		break;
	default:
		break;
	}

	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_CarryObject(Direction _actualDirection)
{
	switch (_actualDirection)
	{
	case NORTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 3) + 624;
		break;
	case SOUTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 0) + 624;
		break;
	case WEST:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 2) + 624;
		break;
	case EAST:
		// PENDENT: FER FLIP
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 1) + 624;
		break;
	default:
		break;
	}

	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_Exercises()
{
}

void Player::Animation_ChangeItems(Direction _actualDirection)
{
	switch (_actualDirection)
	{
	case NORTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 3) + 768;
		break;
	case SOUTH:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 0) + 768;
		break;
	case WEST:
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 2) + 768;
		break;
	case EAST:
		// PENDENT: FER FLIP
		_actualDraw.x = _numFrameAnimation * 16;
		_actualDraw.y = (32 * 1) + 768;
		break;
	default:
		break;
	}

	_actualDraw.w = 16;
	_actualDraw.h = 32;
}

void Player::Animation_Eat()
{
}

#pragma endregion


Player::~Player()
{
}

void Player::LoadJugador()
{
	_idRscMngPlayer = _rscManager->loadAndGetGraphicID("Resources/Images/Jack.png");
}
