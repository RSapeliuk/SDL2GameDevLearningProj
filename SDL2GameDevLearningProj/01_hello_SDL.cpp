#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include "enums\Keys.cpp"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = NULL;

SDL_Texture* loadTexture(std::string path);

SDL_Texture* gTexture = NULL;

SDL_Renderer* gRenderer = NULL;

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL cannot initialize ", SDL_GetError();
	}
	else {

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			std::cout << "Warning Linear texture filtering not enabled!";
		}


		gWindow = SDL_CreateWindow("FirstWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			std::cout << "Window could not be created! SDL_ERROR ", SDL_GetError();
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				std::cout << "Renderer could not be created! SDL_Error: ", SDL_GetError();
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL image could not initialize! SDL_image Error: ", IMG_GetError();
					success = false;
				}
				
			}
			
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	gTexture = loadTexture("images/texture.png");
	if (gTexture == NULL) {
		std::cout << "Failed to load texture image!!!";
		success = false;
	}

	return success;
}

void close() {

	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image!!! SDL Error: ", path.c_str(), IMG_GetError();
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture from!!! SDL Error: ", path.c_str(), SDL_GetError();
		}
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
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


			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					
				}
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				SDL_RenderPresent(gRenderer);

			}

		}
	}
	close();

	return 0;
}