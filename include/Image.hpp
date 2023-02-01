#pragma once

#include <png.h>

#include <Vector2.hpp>
#include <string>

typedef unsigned int color_t;

class Image {
   public:
	Image();
	Image(int _width, int _height);
	Image(std::string const &file);
	~Image();
	Image(Image const &other);
	Image &operator=(Image const &other);

	void Save(std::string const &file);

	color_t GetPixel(Vector2 const &pos) const;

	inline color_t GetPixel(int x, int y) const {
		if (x >= 0 && x < width && y >= 0 && y < height)
			return data[x + width * y];
		return 0;
	}
	inline void SetPixel(int x, int y, const color_t color) {
		data[x + width * y] = color;
	}
	inline void FillColor(color_t color) {
		std::fill(data, data + size, color);
	}
	class FileCantReadException : public std::exception {
		virtual const char *what() const throw();
	};
	class FileCantSaveException : public std::exception {
		virtual const char *what() const throw();
	};
	int width;
	int height;
	png_byte color_type;
	png_byte bit_depth;
	color_t *data;
	int size;
};