#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include "enums\Keys.cpp"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 480;


bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = NULL;

SDL_Surface* loadSurface(std::string path);

SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gCurrentSurface = NULL;

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL cannot initialize " && SDL_GetError();
	}
	else {
		gWindow = SDL_CreateWindow("FirstWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			std::cout << "Window could not be created! SDL_ERROR " && SDL_GetError();
			success = false;
		}
		else
		{
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags)& imgFlags))
			{
				std::cout << "SDL image could not initialize! SDL_image Error: ", IMG_GetError();
				success = false;
			}
			else
			{
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("images/loaded.png");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
		std::cout << "Failed to load default image!!!";
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("images/up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
		std::cout << "Failed to load up image!!!";
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("images/down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
		std::cout << "Failed to load down image!!!";
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("images/left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
		std::cout << "Failed to load LEFT image!!!";
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("images/right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
		std::cout << "Failed to load RIGHT image!!!";
		success = false;
	}

	return success;
}

void close() {
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++)
	{
		SDL_FreeSurface(gKeyPressSurfaces[i]);
		gKeyPressSurfaces[i] = NULL;
	}

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

SDL_Surface* loadSurface(std::string path) {

	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image!!! SDL Error: ", path.c_str(), IMG_GetError();
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			std::cout << "Unable to optimize image!!! SDL Error: ", path.c_str(), SDL_GetError();
		}
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}


int main(int argc, char* args[])
{
	if (!init()) {
		std::cout << "Failed to initialize!";
	}
	else
	{
		if (!loadMedia()) {
			std::cout << "Failed to load media!!!";
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						default:
							gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
				}

				SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;

				SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);

				SDL_UpdateWindowSurface(gWindow);

			}

		}
	}
	close();

	return 0;
}