#include <Render.hpp>
#include <obj_read.hpp>

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

void UpdateAll(Window &window)
{
	Render render(window);
	while (true) {
		Events();
		render.View();
		Benchmark();
	}
}

int main()
{
	Window window("software renderer", 1920, 1080);

	Object object;
	object.mesh = obj_read("test.obj");
	object.transform.rotation =
	    Quaternion(Vector3(0, 180 * ANGLE_TO_RADIAN, 0));
	object.transform.pos = Vector3::Zero();

	Scene::Get().objects.push_back(object);

	Scene::Get().camera.transform.pos = Vector3(0, 0, -3);
	Scene::Get().camera.transform.rotation = Quaternion::Zero();

	while (true) {
		UpdateAll(window);
	}
}