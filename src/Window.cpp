#include <Window.hpp>

Window::Window(std::string name, unsigned int width, unsigned int height)
    : _name(name), _width(width), _height(height)
{
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw WindowError();
	_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	if (!_window)
		throw WindowError();

	SDL_Surface *window_surface = SDL_GetWindowSurface(_window);
	_window_pixels = (unsigned int *)window_surface->pixels;
}

Window::~Window() { SDL_Quit(); }

const char *Window::WindowError::What() const throw() { return SDL_GetError(); }
