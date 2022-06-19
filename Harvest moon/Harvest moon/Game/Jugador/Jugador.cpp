#include "Jugador.h"

Jugador::Jugador(Video* video, ResourceManager* rscManager)
{
	_video = video;
	_rscManager = rscManager;

	_posJugadorX = 0;
	_posJugadorY = 0;

	_direcioActual = SUD;
	_animacioActual = IDLE;
	_frameAnimacio = 0;

	_dibuixActual.h = 0;
	_dibuixActual.w = 0;
	_dibuixActual.x = 0;
	_dibuixActual.y = 0;

	LoadJugador();
}

void Jugador::RenderJugador(int posX, int posY)
{
	AnimacioCaminar(_direcioActual);

	Xywh desti{};
	desti.x = posX;
	desti.y = posY;
	desti.w = _dibuixActual.w;
	desti.h = _dibuixActual.h;

	_video->renderGraphic(_idRscMngJugador, &_dibuixActual, &desti);
}

void Jugador::UpdateJugador()
{
}



#pragma region Animacions

void Jugador::AnimacioCaminar(Direcio _direcioActual)
{
	switch (_direcioActual)
	{
	case NORD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = 32 * 3;
		break;
	case SUD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = 32 * 0;
		break;
	case OEST:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = 32 * 2;
		break;
	case EST:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = 32 * 1;
		break;
	default:
		break;
	}

	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioCorrer(Direcio _direcioActual)
{
	switch (_direcioActual)
	{
	case NORD:
		_dibuixActual.x = (_frameAnimacio * 16) + (16 * 2);
		_dibuixActual.y = 32 * 3;
		break;
	case SUD:
		_dibuixActual.x = (_frameAnimacio * 16) + (16 * 2);
		_dibuixActual.y = 32 * 0;
		break;
	case OEST:
		_dibuixActual.x = (_frameAnimacio * 16) + (16 * 2);
		_dibuixActual.y = 32 * 2;
		break;
	case EST:
		_dibuixActual.x = (_frameAnimacio * 16) + (16 * 2);
		_dibuixActual.y = 32 * 1;
		break;
	default:
		break;
	}

	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioRecollirFruit(Direcio _direcioActual)
{
	switch (_direcioActual)
	{
	case NORD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 3) + 112;
		break;
	case SUD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 0) + 112;
		break;
	case OEST:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 2) + 112;
		break;
	case EST:
		// PENDENT: FER FLIP
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 1) + 112;
		break;
	default:
		break;
	}

	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioSecarSuor()
{
	// Posicio inicial
	_dibuixActual.x = 0;
	_dibuixActual.y = 224;
	// Mida
	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioCansat()
{
	// Posicio inicial
	_dibuixActual.x = 0;
	_dibuixActual.y = 288;
	// Mida
	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioEsgotat()
{
	// Posicio inicial
	_dibuixActual.x = 0;
	_dibuixActual.y = 336;
	// Mida
	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioBeure()
{
	// Posicio inicial
	_dibuixActual.x = 0;
	_dibuixActual.y = 384;
	// Mida
	_dibuixActual.w = 22;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioSaltar(Direcio _direcioActual)
{
	switch (_direcioActual)
	{
	case NORD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 3) + 512;
		break;
	case SUD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 0) + 512;
		break;
	case OEST:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 2) + 512;
		break;
	case EST:
		// PENDENT: FER FLIP
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 1) + 512;
		break;
	default:
		break;
	}

	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioPortarObjecte(Direcio _direcioActual)
{
	switch (_direcioActual)
	{
	case NORD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 3) + 624;
		break;
	case SUD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 0) + 624;
		break;
	case OEST:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 2) + 624;
		break;
	case EST:
		// PENDENT: FER FLIP
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 1) + 624;
		break;
	default:
		break;
	}

	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioExercicis()
{
}

void Jugador::AnimacioCanviItems(Direcio _direcioActual)
{
	switch (_direcioActual)
	{
	case NORD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 3) + 768;
		break;
	case SUD:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 0) + 768;
		break;
	case OEST:
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 2) + 768;
		break;
	case EST:
		// PENDENT: FER FLIP
		_dibuixActual.x = _frameAnimacio * 16;
		_dibuixActual.y = (32 * 1) + 768;
		break;
	default:
		break;
	}

	_dibuixActual.w = 16;
	_dibuixActual.h = 32;
}

void Jugador::AnimacioMenjar()
{
}

#pragma endregion


Jugador::~Jugador()
{
}

void Jugador::LoadJugador()
{
	_idRscMngJugador = _rscManager->loadAndGetGraphicID("Resources/Images/Jack.png");
}
