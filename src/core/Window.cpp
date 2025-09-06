#include "Window.hpp"

#include <format>
#include <stdexcept>

#include <SDL3/SDL_version.h>
#include <SDL3_image/SDL_image.h>
#include <glad/gl.h>

#include "util/FileUtils.hpp"
#include "util/Logger.hpp"

void SDLWindowDeleter::operator()(SDL_Window* w) const
{
	FRACTAL_TRACE("Destroying SDL_Window {}", reinterpret_cast<void*>(w));
	if (w)
	{
		SDL_DestroyWindow(w);
	}
}

Window::Window(std::string_view title, int width, int height)
{
	FRACTAL_TRACE("Window constructor entry: title='{}', width={}, height={}", title, width, height);
	initSDL();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

	FRACTAL_TRACE("Creating SDL window");
	m_window.reset(SDL_CreateWindow(title.data(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED));
	if (!m_window)
	{
		FRACTAL_CRITICAL("Failed to create SDL window: {}", SDL_GetError());
		throw std::runtime_error(std::format("Failed to create SDL window: {}", SDL_GetError()));
	}
	FRACTAL_INFO("SDL window created: {}x{}", width, height);

	const auto iconPath = FileUtils::getAbsolutePath("assets/FractaVista.png");
	SDL_Surface* icon = IMG_Load(iconPath.string().c_str());
	if (icon)
	{
		SDL_SetWindowIcon(m_window.get(), icon);
		SDL_DestroySurface(icon);
		FRACTAL_INFO("Window icon set from '{}'", iconPath.string());
	}
	else
	{
		FRACTAL_WARN("Could not load window icon '{}' : {}", iconPath.string(), SDL_GetError());
	}

	FRACTAL_TRACE("Creating OpenGL context");
	m_glContext = SDL_GL_CreateContext(m_window.get());
	if (!m_glContext)
	{
		FRACTAL_CRITICAL("Failed to create OpenGL context: {}", SDL_GetError());
		throw std::runtime_error(std::format("Failed to create OpenGL context: {}", SDL_GetError()));
	}
	FRACTAL_INFO("OpenGL context created");

	SDL_GL_MakeCurrent(m_window.get(), m_glContext);
	SDL_GL_SetSwapInterval(1);

	initGLAD();
	glViewport(0, 0, width, height);
	FRACTAL_INFO("Initial viewport set to {}x{}", width, height);
}

Window::~Window()
{
	FRACTAL_TRACE("Window destructor entry");
	if (m_glContext)
	{
		SDL_GL_DestroyContext(m_glContext);
		FRACTAL_INFO("OpenGL context destroyed");
	}
	SDL_Quit();
	FRACTAL_INFO("Window destroyed and SDL systems shut down.");
}

void Window::initSDL()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		throw std::runtime_error(std::format("SDL_Init Error: {}", SDL_GetError()));
	}

	const int sdlLinked = SDL_GetVersion();

	FRACTAL_INFO("SDL compiled version: {}.{}.{}  |  SDL linked version: {}.{}.{}", SDL_MAJOR_VERSION,
				 SDL_MINOR_VERSION, SDL_MICRO_VERSION, SDL_VERSIONNUM_MAJOR(sdlLinked), SDL_VERSIONNUM_MINOR(sdlLinked),
				 SDL_VERSIONNUM_MICRO(sdlLinked));

	const int imgLinked = IMG_Version();

	FRACTAL_INFO("SDL_image compiled version: {}.{}.{}  |  SDL_image linked version: {}.{}.{}", SDL_IMAGE_MAJOR_VERSION,
				 SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION, SDL_VERSIONNUM_MAJOR(imgLinked),
				 SDL_VERSIONNUM_MINOR(imgLinked),
				 SDL_VERSIONNUM_MICRO(imgLinked));
}


void Window::initGLAD()
{
	if (!gladLoaderLoadGL())
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}
	FRACTAL_INFO("GLAD initialized. OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
}

void Window::prepareFrame()
{
	int width{}, height{};
	SDL_GetWindowSize(m_window.get(), &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(m_window.get());
}
