#pragma once
#include "../../Engine/Graphics/ResourceManager.h"
#include "../../Engine/Graphics/Video.h"
#include "../Mapa/Mapa.h"
#include "LlistaAnimacions.h"

class Jugador {

public:
	Jugador(Video* video, ResourceManager* rscManager);

	// GET-SET
	void setPosicioX(int posX) { _posJugadorX = posX; }
	void setPosicioY(int posY) { _posJugadorY = posY; }

	// FUNCIONS
	void RenderJugador(int posX, int posY);	
	void UpdateJugador();	

	void AnimacioCaminar(Direcio _direcioActual);
	void AnimacioCorrer(Direcio _direcioActual);
	void AnimacioRecollirFruit(Direcio _direcioActual);
	void AnimacioSecarSuor();
	void AnimacioCansat();
	void AnimacioEsgotat();
	void AnimacioBeure();
	void AnimacioSaltar(Direcio _direcioActual);
	void AnimacioPortarObjecte(Direcio _direcioActual);
	void AnimacioExercicis();
	void AnimacioCanviItems(Direcio _direcioActual);
	void AnimacioMenjar();

	int getIdRscManager() { return _idRscMngJugador; }
	void setIdRscManager(int id) { _idRscMngJugador = id; }		

	~Jugador();


private:
	Video* _video;
	ResourceManager* _rscManager;
	Xywh _dibuixActual;
	int _posJugadorX;
	int _posJugadorY;
	Direcio _direcioActual;
	AnimacioActiva _animacioActual;
	int _frameAnimacio;
	int _idRscMngJugador; // id del grafic al nostre ResourceManager (jugador, ...)

	void LoadJugador();	
};

