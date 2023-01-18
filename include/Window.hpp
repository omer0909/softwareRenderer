#pragma once

#include <SDL.h>
#include <iostream>
#include <png.h>

class Window
{
      public:
	Window(std::string name, unsigned int width, unsigned int height);
	~Window();
	class WindowError : public std::exception
	{
		virtual const char *What() const throw();
	};

	inline unsigned int *GetPixels() const { return _window_pixels; }

	inline void UpdateSurface() { SDL_UpdateWindowSurface(_window); }

	inline unsigned int GetXSize() const { return _width; }
	inline unsigned int GetYSize() const { return _height; }

	inline int Get_pixel(unsigned int x, unsigned int y) const
	{
		return _window_pixels[y * _width + x];
	}

	inline void SetPixel(unsigned int x, unsigned int y, int color)
	{
		_window_pixels[y * _width + x] = color;
	}

	const std::string &GetName() { return _name; }

	void Events();

	inline void Clear(int color)
	{
		for (unsigned int i = 0; i < _width * _height; i++)
			_window_pixels[i] = color;
	}

      private:
	SDL_Window *_window;
	unsigned int *_window_pixels;
	std::string _name;
	unsigned int _width;
	unsigned int _height;
};
