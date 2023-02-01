#include <Render.hpp>
#include <obj_read.hpp>

static int save_png(std::string const &filename, int width, int height,
                    unsigned char *data) {
	int i = 0;
	int r = 0;
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep *row_pointers = NULL;

	fp = fopen(filename.c_str(), "wb");

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	info_ptr = png_create_info_struct(png_ptr);

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA,
	             PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
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
unsigned int screen_to_image(const unsigned int color) {
	int ag = color & 0xFF00FF00;
	int r = (color & 0x00FF0000) >> 16;
	int b = (color & 0x000000FF) << 16;
	return ag | r | b;
}

void Events() {
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	float _deltaTime = Scene::Get().deltaTime;

	constexpr float moveSpeed = 2;
	constexpr float turnSpeed = 2;
	Transform &_cam = Scene::Get().camera.transform;

	if (keystate[SDL_SCANCODE_W])
		_cam.pos = _cam.pos + _cam.Forward() * (moveSpeed * _deltaTime);
	if (keystate[SDL_SCANCODE_S])
		_cam.pos = _cam.pos - _cam.Forward() * (moveSpeed * _deltaTime);
	if (keystate[SDL_SCANCODE_A])
		_cam.pos = _cam.pos + _cam.Right() * (moveSpeed * _deltaTime);
	if (keystate[SDL_SCANCODE_D])
		_cam.pos = _cam.pos - _cam.Right() * (moveSpeed * _deltaTime);
	if (keystate[SDL_SCANCODE_X])
		_cam.pos = _cam.pos + _cam.Up() * (moveSpeed * _deltaTime);
	if (keystate[SDL_SCANCODE_C])
		_cam.pos = _cam.pos - _cam.Up() * (moveSpeed * _deltaTime);

	if (keystate[SDL_SCANCODE_LEFT])
		_cam.rotation = _cam.rotation * (Quaternion(Vector3(0, -45 * ANGLE_TO_RADIAN, 0)).SMultiplay(_deltaTime * turnSpeed));

	if (keystate[SDL_SCANCODE_RIGHT])
		_cam.rotation = _cam.rotation * (Quaternion(Vector3(0, 45 * ANGLE_TO_RADIAN, 0)).SMultiplay(_deltaTime * turnSpeed));

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			exit(0);
		}

		if ((e.type == SDL_KEYDOWN) &&
		    (e.key.keysym.sym == SDLK_ESCAPE)) {
			exit(0);
		}

		if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_p)) {
			Window &_window = *Scene::Get().window;
			unsigned int size = _window.GetYSize() * _window.GetXSize();

			unsigned int *image = new unsigned int[size];
			unsigned int *screen = _window.GetPixels();

			for (unsigned int i = 0; i < size; i++)
				image[i] = screen_to_image(screen[i]);

			save_png("test.png", _window.GetXSize(), _window.GetYSize(), reinterpret_cast<unsigned char *>(image));
			delete[] image;
		}
	}
}

[[gnu::noinline]] void DeltaTimeCalculate() {
	static auto start = std::chrono::system_clock::now();

	auto now = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsed_seconds = now - start;

	std::cout << static_cast<int>(1 / elapsed_seconds.count()) << " fps" << std::endl;

	Scene::Get().deltaTime = elapsed_seconds.count();
	start = now;
}

void JustRender(Render &render) {
	render.View();
	Window &_window = *Scene::Get().window;
	unsigned int size = _window.GetYSize() * _window.GetXSize();

	unsigned int *image = new unsigned int[size];
	unsigned int *screen = _window.GetPixels();

	for (unsigned int i = 0; i < size; i++)
		image[i] = screen_to_image(screen[i]);

	save_png("test.png", _window.GetXSize(), _window.GetYSize(), reinterpret_cast<unsigned char *>(image));
	delete[] image;
	exit(0);
}

/*
unsigned int nonColorGradient(int x)
{
    x /= 6;

    x /= 3;
    x %= 256;
    return x | x << 8 | x << 16;
}

unsigned int colorGradient(int x)
{
    x /= 6;

    int extra = x % 3;
    x /= 3;
    x %= 256;
    return x + (extra > 0 ? 1 : 0) | (x + (extra > 1 ? 1 : 0)) << 8 |
           x << 16;
}
*/
void UpdateAll() {
	/*
	    Window &window = *Scene::Get().window;
	    unsigned int halfHeight = HEIGHT / 2;

	    for (unsigned int y = 0; y < halfHeight; y++) {
	        for (unsigned int x = 0; x < WIDTH; x++) {
	            window.SetPixel(x, y, nonColorGradient(x));
	        }
	    }
	    for (unsigned int y = halfHeight; y < HEIGHT; y++) {
	        for (unsigned int x = 0; x < WIDTH; x++) {
	            window.SetPixel(x, y, colorGradient(x));
	        }
	    }
	    window.UpdateSurface();
	    Events();
	    return;
*/
	Render render(*Scene::Get().window);

	// JustRender(render);

	while (true) {
		Events();
		render.View();
		DeltaTimeCalculate();
	}
}

int main(int argc, char **argv) {
	if (argc != 2 && argc != 3) {
		std::cout << "Argument syntax is: ./program file.obj"
		          << std::endl;
		return -1;
	}

	Window _window("software renderer", WIDTH, HEIGHT);

	Scene::Get().window = &_window;

	Scene::Get().window = &_window;

	if (argc == 3)
		Scene::Get().matCap = Image(argv[2]);

	Object object;
	object.mesh = obj_read(argv[1]);

	object.transform.rotation =
	    Quaternion(Vector3(0, 180 * ANGLE_TO_RADIAN, 0));
	object.transform.pos = Vector3::Zero();

	PointLight pointLight = PointLight();
	pointLight.pos = Vector3(1, 0, -1);
	pointLight.intensity = 100;
	Scene::Get().pointLights.push_back(pointLight);

	// DirectionalLight directionalLight = DirectionalLight();
	// directionalLight.dir = Vector3(0, -1, 0);
	// directionalLight.intensity = 100;
	// Scene::Get().directionalLights.push_back(directionalLight);

	Scene::Get().objects.push_back(object);

	Scene::Get().camera.transform.pos = Vector3(0, 0, -3);
	Scene::Get().camera.transform.rotation = Quaternion::Zero();

	while (true) {
		UpdateAll();
	}
}