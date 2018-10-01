#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>

#include <exception>
#include <iostream>
#include <string>

//Game general information
#define SCREEN_WIDTH 725
#define SCREEN_HEIGHT 432

int main(int, char*[]) 
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (m_window == nullptr) 
		throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (m_renderer == nullptr) 
		throw "No es pot inicialitzar SDL_Renderer";

	//-->SDL_Image 
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	//-->SDL_TTF
	//-->SDL_Mix

	// --- SPRITES ---
		//Background
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg_mainmenu.jpg") };
		if (bgTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		//Cursor
		SDL_Texture* cursorTexture{ IMG_LoadTexture(m_renderer, "../../res/img/greencursor.png") };
		if (cursorTexture == nullptr) throw "Error: cursorTexture init";
		SDL_Rect cursorRect{ 0, 0, 30, 30 };
		//Buttons
		SDL_Texture* btnNormalTexture{ IMG_LoadTexture(m_renderer, "../../res/img/button01_normal.png") };
		if (btnNormalTexture == nullptr) throw "Error: btnNormalTexture init";
		SDL_Texture* btnHoverTexture{ IMG_LoadTexture(m_renderer, "../../res/img/button01_hover.png") };
		if (btnHoverTexture == nullptr) throw "Error: btnHoverTexture init";
		SDL_Texture* btnClickTexture{ IMG_LoadTexture(m_renderer, "../../res/img/button01_click.png") };
		if (btnClickTexture == nullptr) throw "Error: btnClickTexture init";

		SDL_Rect btnPlayRect{ SCREEN_WIDTH / 2 - 246 / 2, SCREEN_HEIGHT / 4 - 67 / 2, 246, 67 };
		SDL_Rect btnSoundRect{ SCREEN_WIDTH / 2 - 246 / 2, SCREEN_HEIGHT / 2 - 67 / 2, 246, 67 };
		SDL_Rect btnQuitRect{ SCREEN_WIDTH / 2 - 246 / 2, 3 * SCREEN_HEIGHT / 4 - 67 / 2, 246, 67 };

		bool mouseOnPlay = false;
		bool mouseOnSound = false;
		bool mouseOnQuit = true;

	//-->Animated Sprite ---
		

	// --- TEXT ---

	// --- AUDIO ---

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	SDL_ShowCursor(SDL_DISABLE);
	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		
				isRunning = false; 
				break;
			case SDL_KEYDOWN:	
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; 
				break;
			default:;
			}
		}

		// UPDATE
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		cursorRect.x = mouseX;
		cursorRect.y = mouseY;
		// DRAW
		SDL_RenderClear(m_renderer);
			//Background
			SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);

			//Buttons
			if ((mouseX < (btnPlayRect.x + btnPlayRect.w) && mouseX > btnPlayRect.x) && 
				(mouseY < (btnPlayRect.y + btnPlayRect.h) && mouseY >(btnPlayRect.y))) 
			{
				//Mouse on play button
				SDL_RenderCopy(m_renderer, btnHoverTexture, nullptr, &btnPlayRect);
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnSoundRect);
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnQuitRect);

				mouseOnPlay = true;
				mouseOnSound = false;
				mouseOnQuit = false;
			}
			else if ((mouseX < (btnSoundRect.x + btnSoundRect.w) && mouseX > btnSoundRect.x) &&
				(mouseY < (btnSoundRect.y + btnSoundRect.h) && mouseY >(btnSoundRect.y)))
			{
				//Mouse on Sound Button
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnPlayRect);
				SDL_RenderCopy(m_renderer, btnHoverTexture, nullptr, &btnSoundRect);
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnQuitRect);

				mouseOnPlay = false;
				mouseOnSound = true;
				mouseOnQuit = false;
			}
			else if ((mouseX < (btnQuitRect.x + btnQuitRect.w) && mouseX > btnQuitRect.x) &&
				(mouseY < (btnQuitRect.y + btnQuitRect.h) && mouseY >(btnQuitRect.y)))
			{
				//Mouse on Quit button
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnPlayRect);
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnSoundRect);
				SDL_RenderCopy(m_renderer, btnHoverTexture, nullptr, &btnQuitRect);

				mouseOnPlay = false;
				mouseOnSound = false;
				mouseOnQuit = true;
			}
			else 
			{
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnPlayRect);
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnSoundRect);
				SDL_RenderCopy(m_renderer, btnNormalTexture, nullptr, &btnQuitRect);

				mouseOnPlay = false;
				mouseOnSound = false;
				mouseOnQuit = true;
			}
			//Cursor
			SDL_RenderCopy(m_renderer, cursorTexture, nullptr, &cursorRect);
		SDL_RenderPresent(m_renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	IMG_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}