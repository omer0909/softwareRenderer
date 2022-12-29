#pragma once

#include <SDL.h>
#include <png.h>
#include <iostream>

class Window
{
      public:
	Window(std::string name, unsigned int width, unsigned int height);
	~Window();
	class WindowError : public std::exception
	{
		virtual const char *What() const throw();
	};
	unsigned int *GetPixels() const;
	void UpdateSurface();
	unsigned int GetXSize() const;
	unsigned int GetYSize() const;

	int Get_pixel(unsigned int x, unsigned int y) const;
	void SetPixel(unsigned int x, unsigned int y, int color);
	const std::string &GetName();
	void Events();
	void Clear(int color);

      private:
	SDL_Window *_window;
	unsigned int *_window_pixels;
	std::string _name;
	unsigned int _width;
	unsigned int _height;
};
