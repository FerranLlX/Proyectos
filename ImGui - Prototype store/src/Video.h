#pragma once
#include "general_headers.h"

class Video_imGui {
	SDL_Window* window;
	ImGuiIO *io;
	SDL_GLContext gl_context;

	Video_imGui();
public:
	static Video_imGui *instance;

	static Video_imGui* getInstance() {
		if (instance == nullptr) {
			instance = new Video_imGui();
		}
		return instance;
	}

	~Video_imGui();

	int init_SDL_openGL_imGui(const char *appname, int w, int h);
	void clearBackground(ImVec4 &color);
	void SwapBuffer() {
		SDL_GL_SwapWindow(window);
	};
	void close();
	// Getters
	SDL_Window* getWindow() { return window; };
	ImGuiIO* getIo() { return io; };
};
