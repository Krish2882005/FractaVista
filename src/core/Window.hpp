#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

struct SDLWindowDeleter
{
		void operator()(SDL_Window* w) const;
};
using SDLWindowPtr = std::unique_ptr<SDL_Window, SDLWindowDeleter>;

class Window
{
	public:
		Window(const std::string& title, int width, int height);
		~Window();

		void prepareFrame();
		void swapBuffers();

		SDL_Window* getSDLWindow() const { return m_window.get(); }
		SDL_GLContext getGLContext() const { return m_glContext; }

	private:
		void initSDL();
		void initOpenGL();
		void initGLAD();

		SDLWindowPtr m_window;
		SDL_GLContext m_glContext = nullptr;
};
