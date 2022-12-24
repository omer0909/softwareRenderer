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

void UpdateAll(Window &window)
{
	clock_t old = clock();
	Render render(window);
	while (true) {
		double deltaTime = (clock() - old) / CLOCKS_PER_SEC;
		(void)deltaTime;
		Events();
		render.View();
		old = clock();
	}
}

int main()
{
	Window window("software renderer", 1920, 1080);

	Object object;
	object.mesh = obj_read("test.obj");
	Scene::Get().objects.push_back(object);

	Scene::Get().camera.transform.pos = Vector3(0, 0, -3);

	while (true) {
		UpdateAll(window);
	}
}