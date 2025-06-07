#include "Window.hpp"

#include <stdexcept>

#include <SDL3_image/SDL_image.h>
#include <glad/glad.h>

#include "utils/Logger.hpp"

void SDLWindowDeleter::operator()(SDL_Window* w) const
{
	if (w)
		SDL_DestroyWindow(w);
}

Window::Window(const std::string& title, int width, int height)
{
	initSDL();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_window.reset(SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

	if (!m_window)
	{
		throw std::runtime_error("Failed to create SDL window.");
	}

	SDL_Surface* icon = IMG_Load("assets/FractaVista.png");
	if (icon)
	{
		SDL_SetWindowIcon(m_window.get(), icon);
		SDL_DestroySurface(icon);
	}
	else
	{
		Logger::Warning("Could not load window icon! SDL_image Error: ", SDL_GetError());
	}

	m_glContext = SDL_GL_CreateContext(m_window.get());
	if (!m_glContext)
	{
		throw std::runtime_error("Failed to create OpenGL context.");
	}

	SDL_GL_MakeCurrent(m_window.get(), m_glContext);
	SDL_GL_SetSwapInterval(1);

	initGLAD();
	initOpenGL();
}

Window::~Window()
{
	if (m_glContext)
	{
		SDL_GL_DestroyContext(m_glContext);
	}
	SDL_Quit();
	Logger::Info("Window destroyed and SDL quit.");
}

void Window::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
}

void Window::initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}
	Logger::Info("GLAD initialized. OpenGL Version: ", glGetString(GL_VERSION));
}

void Window::initOpenGL()
{
	int width, height;
	SDL_GetWindowSize(m_window.get(), &width, &height);
	glViewport(0, 0, width, height);
}

void Window::prepareFrame()
{
	int width, height;
	SDL_GetWindowSize(m_window.get(), &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(m_window.get());
}
