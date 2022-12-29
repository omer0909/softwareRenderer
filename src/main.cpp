#include <Render.hpp>
#include <obj_read.hpp>

static int save_png(std::string const &filename, int width, int height,
		    unsigned char *data)
{
	int i = 0;
	int r = 0;
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep *row_pointers = NULL;

	fp = fopen(filename.c_str(), "wb");

	png_ptr =
	    png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	info_ptr = png_create_info_struct(png_ptr);

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA,
		     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		     PNG_FILTER_TYPE_BASE);
	row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
	for (i = 0; i < height; ++i) {
		row_pointers[i] = data + i * width * 4;
	}
	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, 0, NULL);

	if (NULL != fp) {
		fclose(fp);
		fp = NULL;
	}
	if (NULL != png_ptr) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		png_ptr = NULL;
		info_ptr = NULL;
	}
	if (NULL != row_pointers) {
		free(row_pointers);
		row_pointers = NULL;
	}
	return r;
}

// 0xAARRGGBB
// 0xAABBGGRR
unsigned int screen_to_image(const unsigned int color)
{
	int ag = color & 0xFF00FF00;
	int r = (color & 0x00FF0000) >> 16;
	int b = (color & 0x000000FF) << 16;
	return ag | r | b;
}

void Events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			exit(0);
		}

		if ((e.type == SDL_KEYDOWN) &&
		    (e.key.keysym.sym == SDLK_ESCAPE)) {
			exit(0);
		}

		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_s))
			Scene::Get().camera.transform.pos.z -= 0.1f;
		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_w))
			Scene::Get().camera.transform.pos.z += 0.1f;
		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_a))
			Scene::Get().camera.transform.pos.x += 0.1f;
		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_d))
			Scene::Get().camera.transform.pos.x -= 0.1f;
		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_c))
			Scene::Get().camera.transform.pos.y += 0.1f;
		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_x))
			Scene::Get().camera.transform.pos.y -= 0.1f;

		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_p)) {
			Window &_window = *Scene::Get().window;
			unsigned int size =
			    _window.GetYSize() * _window.GetXSize();

			unsigned int *image = new unsigned int[size];
			unsigned int *screen = _window.GetPixels();

			for (unsigned int i = 0; i < size; i++)
				image[i] = screen_to_image(screen[i]);

			save_png("test.png", _window.GetXSize(),
				 _window.GetYSize(),
				 reinterpret_cast<unsigned char *>(image));
			delete[] image;
		}
	}
}

[[gnu::noinline]] void Benchmark()
{
	static auto start = std::chrono::system_clock::now();

	auto now = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = now - start;

	std::cout << static_cast<int>(1 / elapsed_seconds.count()) << " fps"
		  << std::endl;
	start = now;
}

void UpdateAll()
{
	Render render(*Scene::Get().window);
	while (true) {
		Events();
		render.View();
		Benchmark();
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "Argument syntax is: ./program file.obj"
			  << std::endl;
		return -1;
	}

	Window _window("software renderer", WIDTH, HEIGHT);

	Scene::Get().window = &_window;

	Object object;
	object.mesh = obj_read(argv[1]);
	object.transform.rotation =
	    Quaternion(Vector3(0, 180 * ANGLE_TO_RADIAN, 0));
	object.transform.pos = Vector3::Zero();

	Scene::Get().objects.push_back(object);

	Scene::Get().camera.transform.pos = Vector3(0, 0, -3);
	Scene::Get().camera.transform.rotation = Quaternion::Zero();

	while (true) {
		UpdateAll();
	}
}