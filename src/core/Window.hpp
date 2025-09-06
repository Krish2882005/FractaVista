#pragma once

#include <memory>
#include <string_view>

#include <SDL3/SDL.h>

struct SDLWindowDeleter
{
		void operator()(SDL_Window* w) const;
};
using SDLWindowPtr = std::unique_ptr<SDL_Window, SDLWindowDeleter>;

class Window
{
	public:
		Window(std::string_view title, int width, int height);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		void prepareFrame();
		void swapBuffers();

		[[nodiscard]] SDL_Window* getSDLWindow() const { return m_window.get(); }
		[[nodiscard]] SDL_GLContext getGLContext() const { return m_glContext; }

	private:
		static void initSDL();
		static void initGLAD();

		SDLWindowPtr m_window;
		SDL_GLContext m_glContext = nullptr;
};
