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

int Window::Get_pixel(unsigned int x, unsigned int y) const
{
	return _window_pixels[y * _width + x];
}

void Window::SetPixel(unsigned int x, unsigned int y, int color)
{
	_window_pixels[y * _width + x] = color;
}

unsigned int *Window::GetPixels() const { return _window_pixels; }

void Window::UpdateSurface() { SDL_UpdateWindowSurface(_window); }

Window::~Window() { SDL_Quit(); }

const char *Window::WindowError::What() const throw() { return SDL_GetError(); }

unsigned int Window::GetXSize() const { return _width; }

unsigned int Window::GetYSize() const { return _height; }

void Window::Clear(int color)
{
	for (unsigned int i = 0; i < _width * _height; i++)
		_window_pixels[i] = color;
}
