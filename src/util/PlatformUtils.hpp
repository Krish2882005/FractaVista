#pragma once

#include <nfd.hpp>
#include <optional>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_properties.h>

#include "core/Window.hpp"

namespace PlatformUtils
{
	inline void* getNativeWindowHandle(Window* window)
	{
		SDL_Window* sdlWindow = window->getSDLWindow();

		if (!sdlWindow)
		{
			return nullptr;
		}
		SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
		if (!props)
		{
			return nullptr;
		}

#if defined(SDL_PLATFORM_WINDOWS)
		return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif defined(SDL_PLATFORM_MACOS)
		return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#elif defined(SDL_PLATFORM_LINUX)
		void* handle = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
		if (handle)
		{
			return handle;
		}
		Sint64 xid = SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
		if (xid != 0)
		{
			return (void*)xid;
		}
		return nullptr;
#else
		return nullptr;
#endif
	}

	inline std::optional<std::string> OpenFileDialog(Window* parentWindow, const std::string& title,
													 const std::string& defaultPath,
													 const std::vector<nfdfilteritem_t>& filterPatterns)
	{
		NFD::Guard nfdGuard;

		void* genericHandle = getNativeWindowHandle(parentWindow);
		nfdwindowhandle_t nativeHandle{};
		if (genericHandle)
		{
			nativeHandle.handle = genericHandle;

#if defined(SDL_PLATFORM_WINDOWS)
			nativeHandle.type = NFD_WINDOW_HANDLE_TYPE_WINDOWS;
#elif defined(SDL_PLATFORM_MACOS)
			nativeHandle.type = NFD_WINDOW_HANDLE_TYPE_COCOA;
#elif defined(SDL_PLATFORM_LINUX)
			nativeHandle.type = NFD_WINDOW_HANDLE_TYPE_X11; // Wayland not yet supported
#endif

			nativeHandle.handle = genericHandle;
		}

		NFD::UniquePathU8 outPath;

		nfdresult_t result
			= NFD::OpenDialog(outPath, (const nfdu8filteritem_t*)filterPatterns.data(), filterPatterns.size(),
							  (const nfdu8char_t*)defaultPath.c_str(), nativeHandle);

		if (result == NFD_OKAY)
		{
			return std::string(outPath.get());
		}
		return std::nullopt;
	}

	inline std::optional<std::string> SaveFileDialog(Window* parentWindow, const std::string& title,
													 const std::string& defaultPath,
													 const std::vector<nfdfilteritem_t>& filterPatterns)
	{
		NFD::Guard nfdGuard;

		void* genericHandle = getNativeWindowHandle(parentWindow);

		nfdwindowhandle_t nativeHandle{};
		if (genericHandle)
		{
			nativeHandle.handle = genericHandle;

#if defined(SDL_PLATFORM_WINDOWS)
			nativeHandle.type = NFD_WINDOW_HANDLE_TYPE_WINDOWS;
#elif defined(SDL_PLATFORM_MACOS)
			nativeHandle.type = NFD_WINDOW_HANDLE_TYPE_COCOA;
#elif defined(SDL_PLATFORM_LINUX)
			nativeHandle.type = NFD_WINDOW_HANDLE_TYPE_X11; // Wayland not yet supported
#endif

			nativeHandle.handle = genericHandle;
		}

		NFD::UniquePathU8 outPath;

		nfdresult_t result
			= NFD::SaveDialog(outPath, (const nfdu8filteritem_t*)filterPatterns.data(), filterPatterns.size(),
							  (const nfdu8char_t*)defaultPath.c_str(), nullptr, nativeHandle);

		if (result == NFD_OKAY)
		{
			return std::string(outPath.get());
		}
		return std::nullopt;
	}
}
