#include <stdlib.h>

#include <Image.hpp>

const char *Image::FileCantReadException::what() const throw() {
	return ("File can't read!");
}

const char *Image::FileCantSaveException::what() const throw() {
	return ("File can't save!");
}

Image::Image(int _width, int _height) : width(_width), height(_height), data(new color_t[_width * _height]), size(_width * _height) {
}

Image::Image(Image const &other) {
	*this = other;
}

Image::Image() : data(new color_t[0]), size(0), width(0), height(0) {
}

Image &Image::operator=(Image const &other) {
	if (this == &other)
		return *this;
	delete[] data;
	width = other.width;
	height = other.height;
	color_type = other.color_type;
	bit_depth = other.bit_depth;
	size = other.size;
	data = new color_t[size];
	std::copy(other.data, other.data + other.size, data);
	return *this;
}

// 0xAARRGGBB
// 0xAABBGGRR
inline static unsigned int RGB_to_BGR(const color_t color) {
	int ag = color & 0xFF00FF00;
	int r = (color & 0x00FF0000) >> 16;
	int b = (color & 0x000000FF) << 16;
	return ag | r | b;
}

inline unsigned int BGR_to_RGB(const color_t color) {
	return RGB_to_BGR(color);
}

png_bytep *ImageToRow(color_t *data, int width, int height) {
	png_bytep *row_pointers = new png_bytep[height];
	for (int y = 0; y < height; y++)
		row_pointers[y] = reinterpret_cast<png_bytep>(data + (width * y));
	return row_pointers;
}

inline static int GetRed(const color_t color) {
	return (color & 0x00FF0000) >> 16;
}

inline static int GetGreen(const color_t color) {
	return (color & 0x0000FF00) >> 8;
}

inline static int GetBlue(const color_t color) {
	return (color & 0x000000FF);
}

color_t ColorLerp(color_t A, color_t B, float value) {
	const int aRed = GetRed(A);
	color_t red = (GetRed(B) - aRed) * value + aRed;

	const int aGreen = GetGreen(A);
	color_t green = (GetGreen(B) - aGreen) * value + aGreen;

	const int aBlue = GetBlue(A);
	color_t blue = (GetBlue(B) - aBlue) * value + aBlue;

	return (red << 16) | (green << 8) | blue;
}

color_t Image::GetPixel(Vector2 const &pos) const {
	if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height)
		return 0;
	Vector2 center{std::clamp(pos.x - 0.5f, 0.0f, width - 1.0f),
	               std::clamp(pos.y - 0.5f, 0.0f, height - 1.0f)};

	float floorX = std::floor(center.x);
	float floorY = std::floor(center.y);

	float rateX = center.x - floorX;
	float rateY = center.y - floorY;

	int x = floorX;
	int y = floorY;

	color_t upLeft = GetPixel(x, y);
	color_t upRight = GetPixel(x + 1, y);
	color_t downLeft = GetPixel(x, y + 1);
	color_t downRight = GetPixel(x + 1, y + 1);

	color_t left = ColorLerp(upLeft, downLeft, rateY);
	color_t right = ColorLerp(upRight, downRight, rateY);
	return ColorLerp(left, right, rateX);
}

Image::Image(std::string const &file) {
	FILE *fp = fopen(file.c_str(), "rb");

	if (!fp) throw FileCantReadException();

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) throw FileCantReadException();

	png_infop info = png_create_info_struct(png);
	if (!info) throw FileCantReadException();

	if (setjmp(png_jmpbuf(png))) throw FileCantReadException();

	png_init_io(png, fp);

	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	size = width * height;
	color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
	    color_type == PNG_COLOR_TYPE_GRAY ||
	    color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
	    color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	color_t *BGR_data = new color_t[size];

	png_bytep *row_pointers = ImageToRow(BGR_data, width, height);
	png_read_image(png, row_pointers);
	delete[] row_pointers;

	data = new color_t[size];
	for (int i = 0; i < size; i++)
		data[i] = BGR_to_RGB(BGR_data[i]);
	delete[] BGR_data;

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);
}

void Image::Save(std::string const &file) {
	FILE *fp = fopen(file.c_str(), "wb");
	if (!fp) throw FileCantSaveException();

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) throw FileCantSaveException();

	png_infop info = png_create_info_struct(png);
	if (!info) throw FileCantSaveException();

	if (setjmp(png_jmpbuf(png))) throw FileCantSaveException();

	png_init_io(png, fp);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
	    png,
	    info,
	    width, height,
	    8,
	    PNG_COLOR_TYPE_RGBA,
	    PNG_INTERLACE_NONE,
	    PNG_COMPRESSION_TYPE_DEFAULT,
	    PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	// png_set_filler(png, 0, PNG_FILLER_AFTER);

	if (!data) throw FileCantSaveException();

	color_t *BGR_data = new color_t[size];
	for (int i = 0; i < size; i++)
		BGR_data[i] = RGB_to_BGR(data[i]);

	png_bytep *row_pointers = ImageToRow(BGR_data, width, height);
	png_write_image(png, row_pointers);
	delete[] row_pointers;
	delete[] BGR_data;

	png_write_end(png, NULL);

	fclose(fp);

	png_destroy_write_struct(&png, &info);
}

Image::~Image() {
	delete[] data;
}
