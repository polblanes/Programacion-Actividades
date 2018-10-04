#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <exception>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <unordered_map>

//Game general information
#define SCREEN_WIDTH 848
#define SCREEN_HEIGHT 480

enum class Scene {
	MENU,
	CHSELECT,
	INGAME,
	ENDGAME
};

enum class Directions {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct Number {
	SDL_Texture *tex;
	int w;
	int h;
};

void playerStep(int & current, int & previous) {
	switch (current)
	{
	case 0:
		if (previous == -1)
		{
			current = 1;
		}
		else {
			current = -1;			
		}
		previous = 0;
		break;
	case 1:
		current = 0;
		previous = 1;
		break;
	case -1:
		current = 0;
		previous = -1;
	}	
}
int main(int, char*[]) 
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		throw "No es pot inicialitzar SDL subsystems";

	if (TTF_Init() != 0)
		throw "No es pot inicialitzar SDL_ttf";

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
		///Background
		SDL_Texture* bgMenuTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		if (bgMenuTexture == nullptr) throw "Error: bgMenuTexture init";
		SDL_Texture* bgGameTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bgCastle.jpg") };
		if (bgGameTexture == nullptr) throw "Error: bgGameTexture init";
		SDL_Texture* bgTexture = bgMenuTexture;
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

		///Cursor
		SDL_Texture* cursorTexture{ IMG_LoadTexture(m_renderer, "../../res/img/greencursor.png") };
		if (cursorTexture == nullptr) throw "Error: cursorTexture init";
		SDL_Rect cursorRect{ 0, 0, 30, 30 };

		///Buttons

		//MENU
		SDL_Texture* btnNormalTexture{ IMG_LoadTexture(m_renderer, "../../res/img/button02_normal.png") };
		if (btnNormalTexture == nullptr) throw "Error: btnNormalTexture init";
		SDL_Texture* btnHoverTexture{ IMG_LoadTexture(m_renderer, "../../res/img/button02_hover.png") };
		if (btnHoverTexture == nullptr) throw "Error: btnHoverTexture init";
		SDL_Texture* btnClickTexture{ IMG_LoadTexture(m_renderer, "../../res/img/button02_click.png") };
		if (btnClickTexture == nullptr) throw "Error: btnClickTexture init";

		SDL_Rect btnPlayRect{ SCREEN_WIDTH / 2 - 291 / 2, SCREEN_HEIGHT / 4 - 62 / 2, 291, 62 };
		SDL_Rect btnSoundRect{ SCREEN_WIDTH / 2 - 291 / 2, SCREEN_HEIGHT / 2 - 62 / 2, 291, 62 };
		SDL_Rect btnQuitRect{ SCREEN_WIDTH / 2 - 291 / 2, 3 * SCREEN_HEIGHT / 4 - 62 / 2, 291, 62 };

		//CHARSELECT
		SDL_Texture* charSelBgNormalTexture{ IMG_LoadTexture(m_renderer, "../../res/img/characterBg.png") };
		SDL_Texture* charSelBgSelectedTexture{ IMG_LoadTexture(m_renderer, "../../res/img/selectedCharacterBg.png") };

		SDL_Rect charSelRect00{ SCREEN_WIDTH / 4 - 193, SCREEN_HEIGHT / 3, 92, 92 };
		SDL_Rect charSelRect01{ SCREEN_WIDTH / 4 - 95, SCREEN_HEIGHT / 3, 92, 92 };
		SDL_Rect charSelRect02{ SCREEN_WIDTH / 4 + 3, SCREEN_HEIGHT / 3, 92, 92 };
		SDL_Rect charSelRect03{ SCREEN_WIDTH / 4 + 101, SCREEN_HEIGHT / 3, 92, 92 };

		SDL_Rect charSelRect04{ SCREEN_WIDTH / 4 - 193, SCREEN_HEIGHT / 3 + 98, 92, 92 };
		SDL_Rect charSelRect05{ SCREEN_WIDTH / 4 - 95, SCREEN_HEIGHT / 3 + 98, 92, 92 };
		SDL_Rect charSelRect06{ SCREEN_WIDTH / 4 + 3, SCREEN_HEIGHT / 3 + 98, 92, 92 };
		SDL_Rect charSelRect07{ SCREEN_WIDTH / 4 + 101, SCREEN_HEIGHT / 3 + 98, 92, 92 };

		SDL_Rect charSelRect08{ 3*SCREEN_WIDTH / 4 - 193, SCREEN_HEIGHT / 3, 92, 92 };
		SDL_Rect charSelRect09{ 3*SCREEN_WIDTH / 4 - 95, SCREEN_HEIGHT / 3, 92, 92 };
		SDL_Rect charSelRect10{ 3*SCREEN_WIDTH / 4 + 3, SCREEN_HEIGHT / 3, 92, 92 };
		SDL_Rect charSelRect11{ 3*SCREEN_WIDTH / 4 + 101, SCREEN_HEIGHT / 3, 92, 92 };

		SDL_Rect charSelRect12{ 3*SCREEN_WIDTH / 4 - 193, SCREEN_HEIGHT / 3 + 98, 92, 92 };
		SDL_Rect charSelRect13{ 3*SCREEN_WIDTH / 4 - 95, SCREEN_HEIGHT / 3 + 98, 92, 92 };
		SDL_Rect charSelRect14{ 3*SCREEN_WIDTH / 4 + 3, SCREEN_HEIGHT / 3 + 98, 92, 92 };
		SDL_Rect charSelRect15{ 3*SCREEN_WIDTH / 4 + 101, SCREEN_HEIGHT / 3 + 98, 92, 92 };

		SDL_Rect btnStartRect{ SCREEN_WIDTH / 2 - 291 / 2, 4*SCREEN_HEIGHT / 5 - 62 / 2 + 25, 291, 62 };

		//INGAME
		SDL_Texture* separationBarTexture{ IMG_LoadTexture(m_renderer, "../../res/img/separationBar.png") };

		int horizonHeight = 140;
		SDL_Rect sepBar0{ 0, horizonHeight - 14, 176, 28 };
		SDL_Rect sepBar1{ 176, horizonHeight - 14, 176, 28 };
		SDL_Rect sepBar2{ 352, horizonHeight - 14, 176, 28 };
		SDL_Rect sepBar3{ 528, horizonHeight - 14, 176, 28 };
		SDL_Rect sepBar4{ 704, horizonHeight - 14, 176, 28 };

		SDL_Texture* goldTexture{ IMG_LoadTexture(m_renderer, "../../res/img/gold.png") };
		std::vector<SDL_Rect> goldVector;
		int maxGold = 5;

		///Auxiliares
		SDL_Texture* btnPlayTexture;
		SDL_Texture* btnSoundTexture;
		SDL_Texture* btnQuitTexture;

		SDL_Texture* txtSoundTexture;
		SDL_Rect txtSoundRect;

		SDL_Texture* charSel00Texture;
		SDL_Texture* charSel01Texture;
		SDL_Texture* charSel02Texture;
		SDL_Texture* charSel03Texture;
		SDL_Texture* charSel04Texture;
		SDL_Texture* charSel05Texture;
		SDL_Texture* charSel06Texture;
		SDL_Texture* charSel07Texture;
		SDL_Texture* charSel08Texture;
		SDL_Texture* charSel09Texture;
		SDL_Texture* charSel10Texture;
		SDL_Texture* charSel11Texture;
		SDL_Texture* charSel12Texture;
		SDL_Texture* charSel13Texture;
		SDL_Texture* charSel14Texture;
		SDL_Texture* charSel15Texture;

		SDL_Texture *txtStartTexture;
		SDL_Texture *btnStartTexture;

		///Buton Texts
		const char * font = "../../res/ttf/arial.ttf";
		TTF_Font *arialFont{ TTF_OpenFont(font, 20) };
		if (arialFont == nullptr) throw "No espot inicialitzar TTF_Font";
		
		//MENU
		SDL_Surface	*tmpSurf = TTF_RenderText_Blended(arialFont, "PLAY", SDL_Color{ 255, 255, 255, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtPlayTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		int tmpH, tmpW;
		SDL_QueryTexture(txtPlayTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtPlayRect{ SCREEN_WIDTH / 2 - tmpW / 2, SCREEN_HEIGHT / 4 - tmpH / 2, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "SOUND OFF", SDL_Color{ 255, 255, 255, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtMuteTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtMuteTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtMuteRect{ SCREEN_WIDTH / 2 - tmpW / 2, SCREEN_HEIGHT / 2 - tmpH / 2, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "SOUND ON", SDL_Color{ 255, 255, 255, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtSoundOnTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtMuteTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtSoundOnRect{ SCREEN_WIDTH / 2 - tmpW / 2, SCREEN_HEIGHT / 2 - tmpH / 2, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "QUIT", SDL_Color{ 255, 255, 255, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtQuitTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtQuitTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtQuitRect{ SCREEN_WIDTH / 2 - tmpW / 2, 3*SCREEN_HEIGHT / 4 - tmpH / 2, tmpW, tmpH };

		//CHARSEL
		tmpSurf = TTF_RenderText_Blended(arialFont, "START", SDL_Color{ 255, 255, 255, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtStartOnTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtStartOnTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtStartRect{ SCREEN_WIDTH / 2 - tmpW / 2, 4 * SCREEN_HEIGHT / 5 - tmpH / 2 + 25, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "START", SDL_Color{ 128, 128, 128, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtStartOffTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };

		tmpSurf = TTF_RenderText_Blended(arialFont, "SELECT YOUR CHARACTER!", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtSelectCharacterTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtSelectCharacterTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtSelectCharacterRect{ SCREEN_WIDTH / 2 - tmpW / 2, 1 * SCREEN_HEIGHT / 5 - tmpH / 2 - 50, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "PLAYER ONE", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtPlayerOneTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtPlayerOneTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtPlayerOneRect{ SCREEN_WIDTH / 4 - tmpW / 2, 1 * SCREEN_HEIGHT / 5 - tmpH / 2 + 50, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "PLAYER TWO", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtPlayerTwoTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtPlayerTwoTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtPlayerTwoRect{ 3 * SCREEN_WIDTH / 4 - tmpW / 2, 1 * SCREEN_HEIGHT / 5 - tmpH / 2 + 50, tmpW, tmpH };

		//GAME
		tmpSurf = TTF_RenderText_Blended(arialFont, ":", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtColon{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		int txtColonW, txtColonH;
		SDL_QueryTexture(txtColon, 0, 0, &txtColonW, &txtColonH);

		std::unordered_map<int, Number> numbers;

		tmpSurf = TTF_RenderText_Blended(arialFont, "0", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *zero{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		Number tmpNumber;
		tmpNumber.tex = zero;
		SDL_QueryTexture(zero, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[0] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "1", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *one{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = one;
		SDL_QueryTexture(one, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[1] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "2", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *two{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = two;
		SDL_QueryTexture(two, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[2] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "3", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *three{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = three;
		SDL_QueryTexture(three, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[3] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "4", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *four{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = four;
		SDL_QueryTexture(four, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[4] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "5", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *five{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = five;
		SDL_QueryTexture(five, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[5] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "6", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *six{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = six;
		SDL_QueryTexture(six, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[6] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "7", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *seven{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = seven;
		SDL_QueryTexture(seven, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[7] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "8", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *eight{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = eight;
		SDL_QueryTexture(eight, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[8] = tmpNumber;

		tmpSurf = TTF_RenderText_Blended(arialFont, "9", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *nine{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		tmpNumber.tex = nine;
		SDL_QueryTexture(nine, 0, 0, &tmpNumber.w, &tmpNumber.h);
		numbers[9] = tmpNumber;

		int clockY = 20;

		SDL_Rect colonRect{ SCREEN_WIDTH / 2 - txtColonW / 2, clockY, txtColonW, txtColonH };

		SDL_Rect minTensRect{ 0, clockY, 0, 0 };
		SDL_Rect minUnitsRect{ 0, clockY, 0, 0 };
		SDL_Rect secTensRect{ 0, clockY, 0, 0 };
		SDL_Rect secUnitsRect{ 0, clockY, 0, 0 };

		SDL_Texture *minTensTexture;
		SDL_Texture *minUnitsTexture;
		SDL_Texture *secTensTexture;
		SDL_Texture *secUnitsTexture;

		tmpSurf = TTF_RenderText_Blended(arialFont, "P1:", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtP1Texture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtP1Texture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtP1Rect{ 50 - tmpW / 2, horizonHeight - tmpH / 2 - 100, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "P2:", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtP2Texture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtP2Texture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtP2Rect{ 50 - tmpW / 2, txtP1Rect.y + txtP1Rect.h + 10, tmpW, tmpH };

		SDL_Rect P1PointsTensRect{ 0, txtP1Rect.y, 0, 0 };
		SDL_Rect P1PointsUnitsRect{ 0, txtP1Rect.y, 0, 0 };
		SDL_Rect P2PointsTensRect{ 0, txtP2Rect.y, 0, 0 };
		SDL_Rect P2PointsUnitsRect{ 0, txtP2Rect.y, 0, 0 };

		SDL_Texture *P1PointsTensTexture;
		SDL_Texture *P1PointsUnitsTexture;
		SDL_Texture *P2PointsTensTexture;
		SDL_Texture *P2PointsUnitsTexture;

		//ENDGAME
		tmpSurf = TTF_RenderText_Blended(arialFont, "GAME OVER", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtGameOverTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtGameOverTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtGameOverRect{ SCREEN_WIDTH / 2 - tmpW / 2, - tmpH / 2 + 80, tmpW, tmpH };

		tmpSurf = TTF_RenderText_Blended(arialFont, "PLAYER ONE WINS!", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *P1WinsTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		int p1WinsW, p1WinsH;
		SDL_QueryTexture(P1WinsTexture, 0, 0, &p1WinsW, &p1WinsH);
		
		tmpSurf = TTF_RenderText_Blended(arialFont, "PLAYER TWO WINS!", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *P2WinsTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		int p2WinsW, p2WinsH;
		SDL_QueryTexture(P2WinsTexture, 0, 0, &p2WinsW, &p2WinsH);

		tmpSurf = TTF_RenderText_Blended(arialFont, "DRAW!", SDL_Color{ 0, 0, 0, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *DrawTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		int DrawW, DrawH;
		SDL_QueryTexture(DrawTexture, 0, 0, &DrawW, &DrawH);

		SDL_Texture *WinnerTexture = DrawTexture;
		SDL_Rect WinnerRect{ 0, SCREEN_HEIGHT / 2 - p2WinsH / 2, 0, 0 };

		tmpSurf = TTF_RenderText_Blended(arialFont, "MENU", SDL_Color{ 255, 255, 255, 255 });
		if (tmpSurf == nullptr) throw "Unable to create the SDL text surface";
		SDL_Texture *txtMenuTexture{ SDL_CreateTextureFromSurface(m_renderer, tmpSurf) };
		SDL_QueryTexture(txtMenuTexture, 0, 0, &tmpW, &tmpH);
		SDL_Rect txtMenuRect{ SCREEN_WIDTH / 2 - tmpW / 2, 4 * SCREEN_HEIGHT / 5 - tmpH / 2, tmpW, tmpH };

		SDL_Rect btnMenuRect{ SCREEN_WIDTH / 2 - 291 / 2, 4 * SCREEN_HEIGHT / 5 - 62 / 2, 291, 62 };
		SDL_Texture *btnMenuTexture;


	//-->Animated Sprite ---
		//Player
		SDL_Texture *playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/characters.png") };
		SDL_Texture *playerTextureLocked{ IMG_LoadTexture(m_renderer, "../../res/img/charactersLocked.png") };
		SDL_Rect player1Rect, player1Position;
		SDL_Rect player2Rect, player2Position;

		SDL_Rect p1char0Rect, p1char0position,
			p1char1Rect, p1char1position,
			p1char2Rect, p1char2position,
			p1char3Rect, p1char3position,
			p1char4Rect, p1char4position,
			p1char5Rect, p1char5position,
			p1char6Rect, p1char6position,
			p1char7Rect, p1char7position;

		SDL_Rect p2char0Rect, p2char0position,
			p2char1Rect, p2char1position,
			p2char2Rect, p2char2position,
			p2char3Rect, p2char3position,
			p2char4Rect, p2char4position,
			p2char5Rect, p2char5position,
			p2char6Rect, p2char6position,
			p2char7Rect, p2char7position;

		SDL_Texture * char00Texture;
		SDL_Texture * char01Texture;
		SDL_Texture * char02Texture;
		SDL_Texture * char03Texture;
		SDL_Texture * char04Texture;
		SDL_Texture * char05Texture;
		SDL_Texture * char06Texture;
		SDL_Texture * char07Texture;
		SDL_Texture * char08Texture;
		SDL_Texture * char09Texture;
		SDL_Texture * char10Texture;
		SDL_Texture * char11Texture;
		SDL_Texture * char12Texture;
		SDL_Texture * char13Texture;
		SDL_Texture * char14Texture;
		SDL_Texture * char15Texture;

		int textWidth, textHeight, frameWidth, frameHeight;
		SDL_QueryTexture(playerTexture, NULL, NULL, &textWidth, &textHeight);
		frameWidth = textWidth / 12;
		frameHeight = textHeight / 8;

		player1Position.x = 50;
		player1Position.y = 3 * SCREEN_HEIGHT / 4;
		player1Rect.x = player1Rect.y = 0;
		player1Position.h = player1Rect.h = frameHeight;
		player1Position.w = player1Rect.w = frameWidth;

		int p1currentStep = 0;
		int p1previousStep = -1;
		int p1moveDirection = 0;

		player2Position.x = SCREEN_WIDTH - 50 - 32;
		player2Position.y = 3 * SCREEN_HEIGHT / 4;
		player2Rect.x = player2Rect.y = 0;
		player2Position.h = player2Rect.h = frameHeight;
		player2Position.w = player2Rect.w = frameWidth;

		int p2currentStep = 0;
		int p2previousStep = -1;
		int p2moveDirection = 0;

		p1char0position.x = charSelRect00.x + charSelRect00.w / 2 - frameWidth / 2;
		p1char0position.y = charSelRect00.y + charSelRect00.h / 2 - frameHeight / 2;
		p1char0Rect.x = p1char0Rect.y = 0;
		p1char0position.h = p1char0Rect.h = frameHeight;
		p1char0position.w = p1char0Rect.w = frameWidth;

		p1char1position.x = charSelRect01.x + charSelRect01.w / 2 - frameWidth / 2;
		p1char1position.y = charSelRect01.y + charSelRect01.h / 2 - frameHeight / 2;
		p1char1Rect.x = p1char1Rect.y = 0;
		p1char1position.h = p1char1Rect.h = frameHeight;
		p1char1position.w = p1char1Rect.w = frameWidth;

		p1char2position.x = charSelRect02.x + charSelRect02.w / 2 - frameWidth / 2;
		p1char2position.y = charSelRect02.y + charSelRect02.h / 2 - frameHeight / 2;
		p1char2Rect.x = p1char2Rect.y = 0;
		p1char2position.h = p1char2Rect.h = frameHeight;
		p1char2position.w = p1char2Rect.w = frameWidth;

		p1char3position.x = charSelRect03.x + charSelRect03.w / 2 - frameWidth / 2;
		p1char3position.y = charSelRect03.y + charSelRect03.h / 2 - frameHeight / 2;
		p1char3Rect.x = p1char0Rect.y = 0;
		p1char3position.h = p1char3Rect.h = frameHeight;
		p1char3position.w = p1char3Rect.w = frameWidth;

		p1char4position.x = charSelRect04.x + charSelRect04.w / 2 - frameWidth / 2;
		p1char4position.y = charSelRect04.y + charSelRect04.h / 2 - frameHeight / 2;
		p1char4Rect.x = p1char4Rect.y = 0;
		p1char4position.h = p1char4Rect.h = frameHeight;
		p1char4position.w = p1char4Rect.w = frameWidth;

		p1char5position.x = charSelRect05.x + charSelRect05.w / 2 - frameWidth / 2;
		p1char5position.y = charSelRect05.y + charSelRect05.h / 2 - frameHeight / 2;
		p1char5Rect.x = p1char5Rect.y = 0;
		p1char5position.h = p1char5Rect.h = frameHeight;
		p1char5position.w = p1char5Rect.w = frameWidth;

		p1char6position.x = charSelRect06.x + charSelRect06.w / 2 - frameWidth / 2;
		p1char6position.y = charSelRect06.y + charSelRect06.h / 2 - frameHeight / 2;
		p1char6Rect.x = p1char6Rect.y = 0;
		p1char6position.h = p1char6Rect.h = frameHeight;
		p1char6position.w = p1char6Rect.w = frameWidth;

		p1char7position.x = charSelRect07.x + charSelRect07.w / 2 - frameWidth / 2;
		p1char7position.y = charSelRect07.y + charSelRect07.h / 2 - frameHeight / 2;
		p1char7Rect.x = p1char7Rect.y = 0;
		p1char7position.h = p1char7Rect.h = frameHeight;
		p1char7position.w = p1char7Rect.w = frameWidth;

		p2char0position.x = charSelRect08.x + charSelRect08.w / 2 - frameWidth / 2;
		p2char0position.y = charSelRect08.y + charSelRect08.h / 2 - frameHeight / 2;
		p2char0Rect.x = p2char0Rect.y = 0;
		p2char0position.h = p2char0Rect.h = frameHeight;
		p2char0position.w = p2char0Rect.w = frameWidth;

		p2char1position.x = charSelRect09.x + charSelRect09.w / 2 - frameWidth / 2;
		p2char1position.y = charSelRect09.y + charSelRect09.h / 2 - frameHeight / 2;
		p2char1Rect.x = p2char1Rect.y = 0;
		p2char1position.h = p2char1Rect.h = frameHeight;
		p2char1position.w = p2char1Rect.w = frameWidth;

		p2char2position.x = charSelRect10.x + charSelRect10.w / 2 - frameWidth / 2;
		p2char2position.y = charSelRect10.y + charSelRect10.h / 2 - frameHeight / 2;
		p2char2Rect.x = p2char2Rect.y = 0;
		p2char2position.h = p2char2Rect.h = frameHeight;
		p2char2position.w = p2char2Rect.w = frameWidth;

		p2char3position.x = charSelRect11.x + charSelRect11.w / 2 - frameWidth / 2;
		p2char3position.y = charSelRect11.y + charSelRect11.h / 2 - frameHeight / 2;
		p2char3Rect.x = p2char0Rect.y = 0;
		p2char3position.h = p2char3Rect.h = frameHeight;
		p2char3position.w = p2char3Rect.w = frameWidth;

		p2char4position.x = charSelRect12.x + charSelRect12.w / 2 - frameWidth / 2;
		p2char4position.y = charSelRect12.y + charSelRect12.h / 2 - frameHeight / 2;
		p2char4Rect.x = p2char4Rect.y = 0;
		p2char4position.h = p2char4Rect.h = frameHeight;
		p2char4position.w = p2char4Rect.w = frameWidth;

		p2char5position.x = charSelRect13.x + charSelRect13.w / 2 - frameWidth / 2;
		p2char5position.y = charSelRect13.y + charSelRect13.h / 2 - frameHeight / 2;
		p2char5Rect.x = p2char5Rect.y = 0;
		p2char5position.h = p2char5Rect.h = frameHeight;
		p2char5position.w = p2char5Rect.w = frameWidth;

		p2char6position.x = charSelRect14.x + charSelRect14.w / 2 - frameWidth / 2;
		p2char6position.y = charSelRect14.y + charSelRect14.h / 2 - frameHeight / 2;
		p2char6Rect.x = p2char6Rect.y = 0;
		p2char6position.h = p2char6Rect.h = frameHeight;
		p2char6position.w = p2char6Rect.w = frameWidth;

		p2char7position.x = charSelRect15.x + charSelRect15.w / 2 - frameWidth / 2;
		p2char7position.y = charSelRect15.y + charSelRect15.h / 2 - frameHeight / 2;
		p2char7Rect.x = p2char7Rect.y = 0;
		p2char7position.h = p2char7Rect.h = frameHeight;
		p2char7position.w = p2char7Rect.w = frameWidth;

	// --- TEXT ---

	// --- AUDIO ---

	// --- GAME LOOP ---
	Scene scene = Scene::MENU;
	SDL_Event event;
	bool isRunning = true;
	SDL_ShowCursor(SDL_DISABLE);

	//Input variables
	bool mouseClicked = false;
	int mouseX, mouseY;

	bool escClicked = false;

	bool wDown = false;
	bool aDown = false;
	bool sDown = false;
	bool dDown = false;

	bool prevWDown = false;
	bool prevADown = false;
	bool prevSDown = false;
	bool prevDDown = false;

	bool leftDown = false;
	bool upDown = false;
	bool downDown = false;
	bool rightDown = false;

	bool prevLeftDown = false;
	bool prevUpDown = false;
	bool prevDownDown = false;
	bool prevRightDown = false;

	bool mouseOnPlay = false;
	bool mouseOnSound = false;
	bool mouseOnQuit = false;

	int mouseOnCharacter = -1;

	Directions p1lastDir = Directions::DOWN;
	Directions p1currentDir = Directions::DOWN;
	Directions p2lastDir = Directions::DOWN;
	Directions p2currentDir = Directions::DOWN;

	//Other vars
	bool soundOn = true;
	float ButtonCd = 0.3f;
	float soundTimer = 0.0f;
	float playTimer = 0.0f;
	double previousTime = clock();
	float deltaTime;
	float frameDuration = .1f;
	float p1frameTime = 0.0f;
	float p2frameTime = 0.0f;

	int gameMaxTime = 5;
	int gameTimeLeft = 0;
	int minTens = 0;
	int minUnits = 0;
	int secTens = 0;
	int secUnits = 0;
	float secondTimer = 1.0f;

	//Players
	int p1characterID = -1;
	int p2characterID = -1;
	int mvSpeed = 4;
	bool p1IsMoving = false;
	bool p2IsMoving = false;

	int p1characterX = 0;
	int p1characterY = 0;

	int p2characterX = 0;
	int p2characterY = 0;

	int p1points = 0;
	int p2points = 0;
	
	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) escClicked = true;
				if (event.key.keysym.sym == SDLK_w) wDown = true;
				if (event.key.keysym.sym == SDLK_a) aDown = true;
				if (event.key.keysym.sym == SDLK_s) sDown = true;
				if (event.key.keysym.sym == SDLK_d) dDown = true;
				if (event.key.keysym.sym == SDLK_LEFT) leftDown = true;
				if (event.key.keysym.sym == SDLK_UP) upDown = true;
				if (event.key.keysym.sym == SDLK_RIGHT) rightDown = true;
				if (event.key.keysym.sym == SDLK_DOWN) downDown = true;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE) escClicked = false;
				if (event.key.keysym.sym == SDLK_w) wDown = false;
				if (event.key.keysym.sym == SDLK_a) aDown = false;
				if (event.key.keysym.sym == SDLK_s) sDown = false;
				if (event.key.keysym.sym == SDLK_d) dDown = false;
				if (event.key.keysym.sym == SDLK_LEFT) leftDown = false;
				if (event.key.keysym.sym == SDLK_UP) upDown = false;
				if (event.key.keysym.sym == SDLK_RIGHT) rightDown = false;
				if (event.key.keysym.sym == SDLK_DOWN) downDown = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseClicked = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseClicked = false;
			default:;
			}
		}
		SDL_GetMouseState(&mouseX, &mouseY);

		if (scene == Scene::MENU)
		{
			// UPDATE
			deltaTime = (clock() - previousTime) / float(CLOCKS_PER_SEC);
			previousTime = clock();
			cursorRect.x = mouseX;
			cursorRect.y = mouseY;

			if (soundTimer > 0.0f) {
				soundTimer -= deltaTime;
			}
			else if (soundTimer < 0.0f) {
				soundTimer = 0.0f;
			}
			if (playTimer > 0.0f) {
				playTimer -= deltaTime;
			}
			else if (playTimer < 0.0f) {
				playTimer = 0.0f;
			}

			//ESC
			if (escClicked) isRunning = false;

			//Mouse Click
			if (mouseClicked && mouseOnPlay && playTimer == 0)
			{
				playTimer = ButtonCd;
				scene = Scene::CHSELECT;
			}
			if (mouseOnQuit && mouseClicked) {
				isRunning = false;
			}
			if (mouseOnSound && mouseClicked && soundTimer == 0.0f) {
				soundOn = !soundOn;
				soundTimer = ButtonCd;
			}

			//Buttons
			if ((mouseX < (btnPlayRect.x + btnPlayRect.w) && mouseX > btnPlayRect.x) &&
				(mouseY < (btnPlayRect.y + btnPlayRect.h) && mouseY >(btnPlayRect.y)))
			{
				//Mouse on play button		
				mouseOnPlay = true;
				mouseOnSound = false;
				mouseOnQuit = false;
			}
			else if ((mouseX < (btnSoundRect.x + btnSoundRect.w) && mouseX > btnSoundRect.x) &&
				(mouseY < (btnSoundRect.y + btnSoundRect.h) && mouseY >(btnSoundRect.y)))
			{
				//Mouse on Sound Button
				mouseOnPlay = false;
				mouseOnSound = true;
				mouseOnQuit = false;
			}
			else if ((mouseX < (btnQuitRect.x + btnQuitRect.w) && mouseX > btnQuitRect.x) &&
				(mouseY < (btnQuitRect.y + btnQuitRect.h) && mouseY >(btnQuitRect.y)))
			{
				//Mouse on Quit button
				mouseOnPlay = false;
				mouseOnSound = false;
				mouseOnQuit = true;
			}
			else
			{
				mouseOnPlay = false;
				mouseOnSound = false;
				mouseOnQuit = false;
			}
			if (mouseOnPlay) {
				if (mouseClicked) {
					btnPlayTexture = btnClickTexture;
				}
				else {
					btnPlayTexture = btnHoverTexture;
				}
				btnSoundTexture = btnNormalTexture;
				btnQuitTexture = btnNormalTexture;
			}
			else if (mouseOnSound) {
				if (mouseClicked) {
					btnSoundTexture = btnClickTexture;
				}
				else {
					btnSoundTexture = btnHoverTexture;
				}
				btnPlayTexture = btnNormalTexture;
				btnQuitTexture = btnNormalTexture;
			}
			else if (mouseOnQuit) {
				if (mouseClicked) {
					btnQuitTexture = btnClickTexture;
				}
				else {
					btnQuitTexture = btnHoverTexture;
				}
				btnSoundTexture = btnNormalTexture;
				btnPlayTexture = btnNormalTexture;
			}
			else {
				btnPlayTexture = btnNormalTexture;
				btnSoundTexture = btnNormalTexture;
				btnQuitTexture = btnNormalTexture;
			}

			if (soundOn) {
				txtSoundTexture = txtMuteTexture;
				txtSoundRect = txtMuteRect;
			}
			else {
				txtSoundTexture = txtSoundOnTexture;
				txtSoundRect = txtSoundOnRect;
			}

			// DRAW
			SDL_RenderClear(m_renderer);
			//Background
			SDL_RenderCopy(m_renderer, bgMenuTexture, nullptr, &bgRect);

			//Buttons
			SDL_RenderCopy(m_renderer, btnPlayTexture, nullptr, &btnPlayRect);
			SDL_RenderCopy(m_renderer, btnSoundTexture, nullptr, &btnSoundRect);
			SDL_RenderCopy(m_renderer, btnQuitTexture, nullptr, &btnQuitRect);

			//Text
			SDL_RenderCopy(m_renderer, txtPlayTexture, nullptr, &txtPlayRect);
			SDL_RenderCopy(m_renderer, txtSoundTexture, nullptr, &txtSoundRect);
			SDL_RenderCopy(m_renderer, txtQuitTexture, nullptr, &txtQuitRect);

			//Cursor
			SDL_RenderCopy(m_renderer, cursorTexture, nullptr, &cursorRect);
			SDL_RenderPresent(m_renderer);
		}
		else if (scene == Scene::CHSELECT)
		{
			// UPDATE
		//Animate player		
			deltaTime = (clock() - previousTime) / float(CLOCKS_PER_SEC);
			previousTime = clock();
			cursorRect.x = mouseX;
			cursorRect.y = mouseY;

			if (p1frameTime > 0.0f) {
				p1frameTime -= deltaTime;
			}
			else if (p1frameTime < 0.0f) {
				p1frameTime = 0.0f;
			}
			if (p2frameTime > 0.0f) {
				p2frameTime -= deltaTime;
			}
			else if (p2frameTime < 0.0f) {
				p2frameTime = 0.0f;
			}

			if (soundTimer > 0.0f) {
				soundTimer -= deltaTime;
			}
			else if (soundTimer < 0.0f) {
				soundTimer = 0.0f;
			}
			if (playTimer > 0.0f)
			{
				playTimer -= deltaTime;
			}
			else if (playTimer < 0.0f)
			{
				playTimer = 0.0f;
			}

			if (p1characterID != -1 && p1frameTime == 0.0f)
			{
				playerStep(p1currentStep, p1previousStep);
				p1frameTime = frameDuration;
			}

			if (p2characterID != -1 && p2frameTime == 0.0f)
			{
				playerStep(p2currentStep, p2previousStep);
				p2frameTime = frameDuration;
			}

			p1char0Rect.x = (0 * 3 * frameWidth) + (1 * frameWidth) + (p1characterID == 0 ? p1currentStep : 0) * frameWidth;
			p1char0Rect.y = (0 * 4 * frameHeight) + (p1moveDirection * frameHeight);
			char00Texture = (p2characterID != 0 ? playerTexture : playerTextureLocked);

			p1char1Rect.x = (1 * 3 * frameWidth) + (1 * frameWidth) + (p1characterID == 1 ? p1currentStep : 0) * frameWidth;
			p1char1Rect.y = (0 * 4 * frameHeight) + (p1moveDirection * frameHeight);
			char01Texture = (p2characterID != 1 ? playerTexture : playerTextureLocked);

			p1char2Rect.x = (2 * 3 * frameWidth) + (1 * frameWidth) + (p1characterID == 2 ? p1currentStep : 0) * frameWidth;
			p1char2Rect.y = (0 * 4 * frameHeight) + (p1moveDirection * frameHeight);
			char02Texture = (p2characterID != 2 ? playerTexture : playerTextureLocked);

			p1char3Rect.x = 3 * 3 * frameWidth + 1 * frameWidth + (p1characterID == 3 ? p1currentStep : 0) * frameWidth;
			p1char3Rect.y = 0 * 4 * frameHeight + p1moveDirection * frameHeight;
			char03Texture = p2characterID != 3 ? playerTexture : playerTextureLocked;

			p1char4Rect.x = 0 * 3 * frameWidth + 1 * frameWidth + (p1characterID == 4 ? p1currentStep : 0) * frameWidth;
			p1char4Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char04Texture = p2characterID != 4 ? playerTexture : playerTextureLocked;

			p1char5Rect.x = 1 * 3 * frameWidth + 1 * frameWidth + (p1characterID == 5 ? p1currentStep : 0) * frameWidth;
			p1char5Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char05Texture = p2characterID != 5 ? playerTexture : playerTextureLocked;

			p1char6Rect.x = 2 * 3 * frameWidth + 1 * frameWidth + (p1characterID == 6 ? p1currentStep : 0) * frameWidth;
			p1char6Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char06Texture = p2characterID != 6 ? playerTexture : playerTextureLocked;

			p1char7Rect.x = 3 * 3 * frameWidth + 1 * frameWidth + (p1characterID == 7 ? p1currentStep : 0) * frameWidth;
			p1char7Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char07Texture = p2characterID != 7 ? playerTexture : playerTextureLocked;

			p2char0Rect.x = 0 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 0 ? p2currentStep : 0) * frameWidth;
			p2char0Rect.y = 0 * 4 * frameHeight + p1moveDirection * frameHeight;
			char08Texture = p1characterID != 0 ? playerTexture : playerTextureLocked;

			p2char1Rect.x = 1 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 1 ? p2currentStep : 0) * frameWidth;
			p2char1Rect.y = 0 * 4 * frameHeight + p1moveDirection * frameHeight;
			char09Texture = p1characterID != 1 ? playerTexture : playerTextureLocked;

			p2char2Rect.x = 2 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 2 ? p2currentStep : 0) * frameWidth;
			p2char2Rect.y = 0 * 4 * frameHeight + p1moveDirection * frameHeight;
			char10Texture = p1characterID != 2 ? playerTexture : playerTextureLocked;

			p2char3Rect.x = 3 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 3 ? p2currentStep : 0) * frameWidth;
			p2char3Rect.y = 0 * 4 * frameHeight + p1moveDirection * frameHeight;
			char11Texture = p1characterID != 3 ? playerTexture : playerTextureLocked;

			p2char4Rect.x = 0 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 4 ? p2currentStep : 0) * frameWidth;
			p2char4Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char12Texture = p1characterID != 4 ? playerTexture : playerTextureLocked;

			p2char5Rect.x = 1 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 5 ? p2currentStep : 0) * frameWidth;
			p2char5Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char13Texture = p1characterID != 5 ? playerTexture : playerTextureLocked;

			p2char6Rect.x = 2 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 6 ? p2currentStep : 0) * frameWidth;
			p2char6Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char14Texture = p1characterID != 6 ? playerTexture : playerTextureLocked;

			p2char7Rect.x = 3 * 3 * frameWidth + 1 * frameWidth + (p2characterID == 7 ? p2currentStep : 0) * frameWidth;
			p2char7Rect.y = 1 * 4 * frameHeight + p1moveDirection * frameHeight;
			char15Texture = p1characterID != 7 ? playerTexture : playerTextureLocked;

			//ESC
			if (escClicked)
			{
				scene = Scene::MENU;
				escClicked = false;
			}

			//Mouse Clicked
			if (mouseClicked && mouseOnPlay && playTimer == 0)
			{
				playTimer = ButtonCd;
				switch (p1characterID)
				{
				case 0:
					p1characterX = 0;
					p1characterY = 0;
					break;
				case 1:
					p1characterX = 1;
					p1characterY = 0;
					break;
				case 2:
					p1characterX = 2;
					p1characterY = 0;
					break;
				case 3:
					p1characterX = 3;
					p1characterY = 0;
					break;
				case 4:
					p1characterX = 0;
					p1characterY = 1;
					break;
				case 5:
					p1characterX = 1;
					p1characterY = 1;
					break;
				case 6:
					p1characterX = 2;
					p1characterY = 1;
					break;
				case 7:
					p1characterX = 3;
					p1characterY = 1;
					break;
				}
				switch (p2characterID)
				{
				case 0:
					p2characterX = 0;
					p2characterY = 0;
					break;
				case 1:
					p2characterX = 1;
					p2characterY = 0;
					break;
				case 2:
					p2characterX = 2;
					p2characterY = 0;
					break;
				case 3:
					p2characterX = 3;
					p2characterY = 0;
					break;
				case 4:
					p2characterX = 0;
					p2characterY = 1;
					break;
				case 5:
					p2characterX = 1;
					p2characterY = 1;
					break;
				case 6:
					p2characterX = 2;
					p2characterY = 1;
					break;
				case 7:
					p2characterX = 3;
					p2characterY = 1;
					break;
				}

				gameTimeLeft = gameMaxTime;

				scene = Scene::INGAME;
			}

			//Buttons

			if ((mouseX < (btnStartRect.x + btnStartRect.w) && mouseX > btnStartRect.x) &&
				(mouseY < (btnStartRect.y + btnStartRect.h) && mouseY >(btnStartRect.y)))
			{
				mouseOnPlay = true;
			}
			else
			{
				mouseOnPlay = false;
			}
			if ((mouseX < (charSelRect00.x + charSelRect00.w) && mouseX > charSelRect00.x) &&
				(mouseY < (charSelRect00.y + charSelRect00.h) && mouseY >(charSelRect00.y)))
			{
				mouseOnCharacter = 0;
			}
			else if ((mouseX < (charSelRect01.x + charSelRect01.w) && mouseX > charSelRect01.x) &&
				(mouseY < (charSelRect01.y + charSelRect01.h) && mouseY >(charSelRect01.y)))
			{
				mouseOnCharacter = 1;
			}
			else if ((mouseX < (charSelRect02.x + charSelRect02.w) && mouseX > charSelRect02.x) &&
				(mouseY < (charSelRect02.y + charSelRect02.h) && mouseY >(charSelRect02.y)))
			{
				mouseOnCharacter = 2;
			}
			else if ((mouseX < (charSelRect03.x + charSelRect03.w) && mouseX > charSelRect03.x) &&
				(mouseY < (charSelRect03.y + charSelRect03.h) && mouseY >(charSelRect03.y)))
			{
				mouseOnCharacter = 3;
			}
			else if ((mouseX < (charSelRect04.x + charSelRect04.w) && mouseX > charSelRect04.x) &&
				(mouseY < (charSelRect04.y + charSelRect04.h) && mouseY >(charSelRect04.y)))
			{
				mouseOnCharacter = 4;
			}
			else if ((mouseX < (charSelRect05.x + charSelRect05.w) && mouseX > charSelRect05.x) &&
				(mouseY < (charSelRect05.y + charSelRect05.h) && mouseY >(charSelRect05.y)))
			{
				mouseOnCharacter = 5;
			}
			else if ((mouseX < (charSelRect06.x + charSelRect06.w) && mouseX > charSelRect06.x) &&
				(mouseY < (charSelRect06.y + charSelRect06.h) && mouseY >(charSelRect06.y)))
			{
				mouseOnCharacter = 6;
			}
			else if ((mouseX < (charSelRect07.x + charSelRect07.w) && mouseX > charSelRect07.x) &&
				(mouseY < (charSelRect07.y + charSelRect07.h) && mouseY >(charSelRect07.y)))
			{
				mouseOnCharacter = 7;
			}
			else if ((mouseX < (charSelRect08.x + charSelRect08.w) && mouseX > charSelRect08.x) &&
				(mouseY < (charSelRect08.y + charSelRect08.h) && mouseY >(charSelRect08.y)))
			{
				mouseOnCharacter = 8;
			}
			else if ((mouseX < (charSelRect09.x + charSelRect09.w) && mouseX > charSelRect09.x) &&
				(mouseY < (charSelRect09.y + charSelRect09.h) && mouseY >(charSelRect09.y)))
			{
				mouseOnCharacter = 9;
			}
			else if ((mouseX < (charSelRect10.x + charSelRect10.w) && mouseX > charSelRect10.x) &&
				(mouseY < (charSelRect10.y + charSelRect10.h) && mouseY >(charSelRect10.y)))
			{
				mouseOnCharacter = 10;
			}
			else if ((mouseX < (charSelRect11.x + charSelRect11.w) && mouseX > charSelRect11.x) &&
				(mouseY < (charSelRect11.y + charSelRect11.h) && mouseY >(charSelRect11.y)))
			{
				mouseOnCharacter = 11;
			}
			else if ((mouseX < (charSelRect12.x + charSelRect12.w) && mouseX > charSelRect12.x) &&
				(mouseY < (charSelRect12.y + charSelRect12.h) && mouseY >(charSelRect12.y)))
			{
				mouseOnCharacter = 12;
			}
			else if ((mouseX < (charSelRect13.x + charSelRect13.w) && mouseX > charSelRect13.x) &&
				(mouseY < (charSelRect13.y + charSelRect13.h) && mouseY >(charSelRect13.y)))
			{
				mouseOnCharacter = 13;
			}
			else if ((mouseX < (charSelRect14.x + charSelRect14.w) && mouseX > charSelRect14.x) &&
				(mouseY < (charSelRect14.y + charSelRect14.h) && mouseY >(charSelRect14.y)))
			{
				mouseOnCharacter = 14;
			}
			else if ((mouseX < (charSelRect15.x + charSelRect15.w) && mouseX > charSelRect15.x) &&
				(mouseY < (charSelRect15.y + charSelRect15.h) && mouseY >(charSelRect15.y)))
			{
				mouseOnCharacter = 15;
			}
			else
			{
				mouseOnCharacter = -1;
			}

			if (mouseClicked && mouseOnCharacter == 0 && p2characterID != 0)
			{
				p1characterID = 0;
			}
			else if (mouseClicked && mouseOnCharacter == 1 && p2characterID != 1)
			{
				p1characterID = 1;
			}
			else if (mouseClicked && mouseOnCharacter == 2 && p2characterID != 2)
			{
				p1characterID = 2;
			}
			else if (mouseClicked && mouseOnCharacter == 3 && p2characterID != 3)
			{
				p1characterID = 3;
			}
			else if (mouseClicked && mouseOnCharacter == 4 && p2characterID != 4)
			{
				p1characterID = 4;
			}
			else if (mouseClicked && mouseOnCharacter == 5 && p2characterID != 5)
			{
				p1characterID = 5;
			}
			else if (mouseClicked && mouseOnCharacter == 6 && p2characterID != 6)
			{
				p1characterID = 6;
			}
			else if (mouseClicked && mouseOnCharacter == 7 && p2characterID != 7)
			{
				p1characterID = 7;
			}
			else if (mouseClicked && mouseOnCharacter == 8 && p1characterID != 0)
			{
				p2characterID = 0;
			}
			else if (mouseClicked && mouseOnCharacter == 9 && p1characterID != 1)
			{
				p2characterID = 1;
			}
			else if (mouseClicked && mouseOnCharacter == 10 && p1characterID != 2)
			{
				p2characterID = 2;
			}
			else if (mouseClicked && mouseOnCharacter == 11 && p1characterID != 3)
			{
				p2characterID = 3;
			}
			else if (mouseClicked && mouseOnCharacter == 12 && p1characterID != 4)
			{
				p2characterID = 4;
			}
			else if (mouseClicked && mouseOnCharacter == 13 && p1characterID != 5)
			{
				p2characterID = 5;
			}
			else if (mouseClicked && mouseOnCharacter == 14 && p1characterID != 6)
			{
				p2characterID = 6;
			}
			else if (mouseClicked && mouseOnCharacter == 15 && p1characterID != 7)
			{
				p2characterID = 7;
			}

			charSel01Texture = charSelBgNormalTexture;
			charSel02Texture = charSelBgNormalTexture;
			charSel03Texture = charSelBgNormalTexture;
			charSel04Texture = charSelBgNormalTexture;
			charSel05Texture = charSelBgNormalTexture;
			charSel06Texture = charSelBgNormalTexture;
			charSel07Texture = charSelBgNormalTexture;
			charSel08Texture = charSelBgNormalTexture;
			charSel09Texture = charSelBgNormalTexture;
			charSel10Texture = charSelBgNormalTexture;
			charSel11Texture = charSelBgNormalTexture;
			charSel12Texture = charSelBgNormalTexture;
			charSel13Texture = charSelBgNormalTexture;
			charSel14Texture = charSelBgNormalTexture;
			charSel15Texture = charSelBgNormalTexture;
			charSel00Texture = charSelBgNormalTexture;

			if (p1characterID != -1)
			{
				switch (p1characterID) {
				case 0:
					charSel00Texture = charSelBgSelectedTexture;
					break;
				case 1:
					charSel01Texture = charSelBgSelectedTexture;
					break;
				case 2:
					charSel02Texture = charSelBgSelectedTexture;
					break;
				case 3:
					charSel03Texture = charSelBgSelectedTexture;
					break;
				case 4:
					charSel04Texture = charSelBgSelectedTexture;
					break;
				case 5:
					charSel05Texture = charSelBgSelectedTexture;
					break;
				case 6:
					charSel06Texture = charSelBgSelectedTexture;
					break;
				case 7:
					charSel07Texture = charSelBgSelectedTexture;
					break;
				}
			}
			if (p2characterID != -1)
			{
				switch (p2characterID) {
				case 0:
					charSel08Texture = charSelBgSelectedTexture;
					break;
				case 1:
					charSel09Texture = charSelBgSelectedTexture;
					break;
				case 2:
					charSel10Texture = charSelBgSelectedTexture;
					break;
				case 3:
					charSel11Texture = charSelBgSelectedTexture;
					break;
				case 4:
					charSel12Texture = charSelBgSelectedTexture;
					break;
				case 5:
					charSel13Texture = charSelBgSelectedTexture;
					break;
				case 6:
					charSel14Texture = charSelBgSelectedTexture;
					break;
				case 7:
					charSel15Texture = charSelBgSelectedTexture;
					break;
				}
			}

			if (p1characterID != -1 && p2characterID != -1)
			{
				//Both characters selected
				if (mouseOnPlay)
				{
					if (mouseClicked)
					{
						btnStartTexture = btnClickTexture;
					}
					else
					{
						btnStartTexture = btnHoverTexture;
					}
				}
				else
				{
					btnStartTexture = btnNormalTexture;
				}
				txtStartTexture = txtStartOnTexture;
			}
			else
			{
				txtStartTexture = txtStartOffTexture;
				btnStartTexture = btnNormalTexture;
			}

			// DRAW
			SDL_RenderClear(m_renderer);
			//Background
			SDL_RenderCopy(m_renderer, bgGameTexture, nullptr, &bgRect);

			//Buttons
			SDL_RenderCopy(m_renderer, charSel00Texture, nullptr, &charSelRect00);
			SDL_RenderCopy(m_renderer, charSel01Texture, nullptr, &charSelRect01);
			SDL_RenderCopy(m_renderer, charSel02Texture, nullptr, &charSelRect02);
			SDL_RenderCopy(m_renderer, charSel03Texture, nullptr, &charSelRect03);
			SDL_RenderCopy(m_renderer, charSel04Texture, nullptr, &charSelRect04);
			SDL_RenderCopy(m_renderer, charSel05Texture, nullptr, &charSelRect05);
			SDL_RenderCopy(m_renderer, charSel06Texture, nullptr, &charSelRect06);
			SDL_RenderCopy(m_renderer, charSel07Texture, nullptr, &charSelRect07);
			SDL_RenderCopy(m_renderer, charSel08Texture, nullptr, &charSelRect08);
			SDL_RenderCopy(m_renderer, charSel09Texture, nullptr, &charSelRect09);
			SDL_RenderCopy(m_renderer, charSel10Texture, nullptr, &charSelRect10);
			SDL_RenderCopy(m_renderer, charSel11Texture, nullptr, &charSelRect11);
			SDL_RenderCopy(m_renderer, charSel12Texture, nullptr, &charSelRect12);
			SDL_RenderCopy(m_renderer, charSel13Texture, nullptr, &charSelRect13);
			SDL_RenderCopy(m_renderer, charSel14Texture, nullptr, &charSelRect14);
			SDL_RenderCopy(m_renderer, charSel15Texture, nullptr, &charSelRect15);

			SDL_RenderCopy(m_renderer, char00Texture, &p1char0Rect, &p1char0position);
			SDL_RenderCopy(m_renderer, char01Texture, &p1char1Rect, &p1char1position);
			SDL_RenderCopy(m_renderer, char02Texture, &p1char2Rect, &p1char2position);
			SDL_RenderCopy(m_renderer, char03Texture, &p1char3Rect, &p1char3position);
			SDL_RenderCopy(m_renderer, char04Texture, &p1char4Rect, &p1char4position);
			SDL_RenderCopy(m_renderer, char05Texture, &p1char5Rect, &p1char5position);
			SDL_RenderCopy(m_renderer, char06Texture, &p1char6Rect, &p1char6position);
			SDL_RenderCopy(m_renderer, char07Texture, &p1char7Rect, &p1char7position);

			SDL_RenderCopy(m_renderer, char08Texture, &p2char0Rect, &p2char0position);
			SDL_RenderCopy(m_renderer, char09Texture, &p2char1Rect, &p2char1position);
			SDL_RenderCopy(m_renderer, char10Texture, &p2char2Rect, &p2char2position);
			SDL_RenderCopy(m_renderer, char11Texture, &p2char3Rect, &p2char3position);
			SDL_RenderCopy(m_renderer, char12Texture, &p2char4Rect, &p2char4position);
			SDL_RenderCopy(m_renderer, char13Texture, &p2char5Rect, &p2char5position);
			SDL_RenderCopy(m_renderer, char14Texture, &p2char6Rect, &p2char6position);
			SDL_RenderCopy(m_renderer, char15Texture, &p2char7Rect, &p2char7position);

			SDL_RenderCopy(m_renderer, btnStartTexture, nullptr, &btnStartRect);

			//Text
			SDL_RenderCopy(m_renderer, txtStartTexture, nullptr, &txtStartRect);
			SDL_RenderCopy(m_renderer, txtSelectCharacterTexture, nullptr, &txtSelectCharacterRect);
			SDL_RenderCopy(m_renderer, txtPlayerOneTexture, nullptr, &txtPlayerOneRect);
			SDL_RenderCopy(m_renderer, txtPlayerTwoTexture, nullptr, &txtPlayerTwoRect);
			//Cursor
			SDL_RenderCopy(m_renderer, cursorTexture, nullptr, &cursorRect);
			SDL_RenderPresent(m_renderer);
		}
		else if (scene == Scene::INGAME)
		{
			// UPDATE
			deltaTime = (clock() - previousTime) / float(CLOCKS_PER_SEC);
			previousTime = clock();

			if (secondTimer > 0.0f)
			{
				secondTimer -= deltaTime;
			}
			else
			{
				if (gameTimeLeft > 0)
				{
					gameTimeLeft--;
				}
				else
				{
					if (p1points > p2points)
					{
						//P1 Wins
						WinnerTexture = P1WinsTexture;
						WinnerRect.x = SCREEN_WIDTH / 2 - p1WinsW / 2;
						WinnerRect.w = p1WinsW;
						WinnerRect.h = p1WinsH;
					}
					else if (p1points < p2points)
					{
						//P2 Wins
						WinnerTexture = P2WinsTexture;
						WinnerRect.x = SCREEN_WIDTH / 2 - p2WinsW / 2;
						WinnerRect.w = p2WinsW;
						WinnerRect.h = p2WinsH;
					}
					else
					{
						//Draw
						WinnerTexture = DrawTexture;
						WinnerRect.x = SCREEN_WIDTH / 2 - DrawW / 2;
						WinnerRect.w = DrawW;
						WinnerRect.h = DrawH;
					}
					scene = Scene::ENDGAME;
				}
				secondTimer = 1.0f;
			}
			minTens = floor(gameTimeLeft / 600);
			minUnits = floor((gameTimeLeft - minTens * 600) / 60);
			secTens = floor((gameTimeLeft - minTens * 600 - minUnits * 60) / 10);
			secUnits = gameTimeLeft - minTens * 600 - minUnits * 60 - secTens * 10;

			if (p1frameTime > 0.0f) {
				p1frameTime -= deltaTime;
			}
			else if (p1frameTime < 0.0f) {
				p1frameTime = 0.0f;
			}
			if (p2frameTime > 0.0f) {
				p2frameTime -= deltaTime;
			}
			else if (p2frameTime < 0.0f) {
				p2frameTime = 0.0f;
			}

			if (soundTimer > 0.0f) {
				soundTimer -= deltaTime;
			}
			else if (soundTimer < 0.0f) {
				soundTimer = 0.0f;
			}

			if (p1IsMoving && p1frameTime == 0.0f)
			{
				playerStep(p1currentStep, p1previousStep);
				p1frameTime = frameDuration;
			}

			if (p2IsMoving != -1 && p2frameTime == 0.0f)
			{
				playerStep(p2currentStep, p2previousStep);
				p2frameTime = frameDuration;
			}

			while (goldVector.size() < maxGold)
			{
				goldVector.push_back(SDL_Rect{ rand() % SCREEN_WIDTH, horizonHeight + rand() % (SCREEN_HEIGHT - horizonHeight - 32), 32, 32 });
			}

			//ESC
			if (escClicked)
			{
				scene = Scene::MENU;
				escClicked = false;
			}

			//Keysboard
			if (wDown || sDown || aDown || dDown)
			{
				p1IsMoving = true;
			}
			else
			{
				p1IsMoving = false;
			}

			if (upDown || downDown || leftDown || rightDown)
			{
				p2IsMoving = true;
			}
			else
			{
				p2IsMoving = false;
			}

			if (wDown && !prevWDown)
			{
				p1currentDir = Directions::UP;
				p1moveDirection = 3;
			}
			if (sDown && !prevSDown)
			{
				p1currentDir = Directions::DOWN;
				p1moveDirection = 0;
			}
			if (aDown && !prevADown)
			{
				p1currentDir = Directions::LEFT;
				p1moveDirection = 1;
			}
			if (dDown && !prevDDown)
			{
				p1currentDir = Directions::RIGHT;
				p1moveDirection = 2;
			}

			if (upDown && !prevUpDown)
			{
				p2currentDir = Directions::UP;
				p2moveDirection = 3;
			}
			if (downDown && !prevDownDown)
			{
				p2currentDir = Directions::DOWN;
				p2moveDirection = 0;
			}
			if (leftDown && !prevLeftDown)
			{
				p2currentDir = Directions::LEFT;
				p2moveDirection = 1;
			}
			if (rightDown && !prevRightDown)
			{
				p2currentDir = Directions::RIGHT;
				p2moveDirection = 2;
			}

			if (p1IsMoving)
			{
				switch (p1currentDir)
				{
				case Directions::UP:
					if (player1Position.y > horizonHeight - mvSpeed)
					{
						player1Position.y -= mvSpeed;
					}
					else if (player1Position.y > horizonHeight)
					{
						player1Position.y = horizonHeight;
					}
					break;
				case Directions::DOWN:
					if (player1Position.y < SCREEN_HEIGHT - 32 - mvSpeed)
					{
						player1Position.y += mvSpeed;
					}
					else if (player1Position.y < SCREEN_HEIGHT - 32)
					{
						player1Position.y = SCREEN_HEIGHT - 32;
					}
					break;
				case Directions::RIGHT:
					if (player1Position.x >= SCREEN_WIDTH)
					{
						player1Position.x = player1Position.x + mvSpeed - SCREEN_WIDTH - 32;
					}
					else
					{
						player1Position.x += mvSpeed;
					}
					break;
				case Directions::LEFT:					
					if (player1Position.x <= -32 + mvSpeed)
					{
						player1Position.x = player1Position.x - mvSpeed + SCREEN_WIDTH + 32;
					}
					else
					{
						player1Position.x -= mvSpeed;
					}
					break;
				}
			}

			if (p2IsMoving)
			{
				switch (p2currentDir)
				{
				case Directions::UP:
					if (player2Position.y > horizonHeight - mvSpeed)
					{
						player2Position.y -= mvSpeed;
					}
					else if (player2Position.y > horizonHeight)
					{
						player2Position.y = horizonHeight;
					}
					break;
				case Directions::DOWN:
					if (player2Position.y < SCREEN_HEIGHT - 32 - mvSpeed)
					{
						player2Position.y += mvSpeed;
					}
					else if (player2Position.y < SCREEN_HEIGHT - 32)
					{
						player2Position.y = SCREEN_HEIGHT - 32;
					}
					break;
				case Directions::RIGHT:
					if (player2Position.x >= SCREEN_WIDTH)
					{
						player2Position.x = player2Position.x + mvSpeed - SCREEN_WIDTH - 32;
					}
					else
					{
						player2Position.x += mvSpeed;
					}
					break;
				case Directions::LEFT:
					if (player2Position.x <= -32 + mvSpeed)
					{
						player2Position.x = player2Position.x - mvSpeed + SCREEN_WIDTH + 32;
					}
					else
					{
						player2Position.x -= mvSpeed;
					}					
					break;
				}
			}

			int destroyGoldIndex = -1;
			for (int i = 0; i < goldVector.size(); ++i)
			{
				if (abs(player1Position.x - goldVector[i].x) < 32 && abs(player1Position.y - goldVector[i].y) < 32)
				{
					p1points++;
					destroyGoldIndex = i;
					break;
				}
			}
			if (destroyGoldIndex != -1)
			{
				goldVector.erase(goldVector.begin() + destroyGoldIndex);
			}

			destroyGoldIndex = -1;
			for (int i = 0; i < goldVector.size(); ++i)
			{
				if (abs(player2Position.x - goldVector[i].x) < 32 && abs(player2Position.y - goldVector[i].y) < 32)
				{
					p2points++;
					destroyGoldIndex = i;
					break;
				}
			}
			if (destroyGoldIndex != -1)
			{
				goldVector.erase(goldVector.begin() + destroyGoldIndex);
			}

			player1Rect.x = (p1characterX * 3 * frameWidth) + (1 * frameWidth) + (p1IsMoving ? p1currentStep : 0) * frameWidth;
			player1Rect.y = (p1characterY * 4 * frameHeight) + (p1moveDirection * frameHeight);

			player2Rect.x = (p2characterX * 3 * frameWidth) + (1 * frameWidth) + (p2IsMoving ? p2currentStep : 0) * frameWidth;
			player2Rect.y = (p2characterY * 4 * frameHeight) + (p2moveDirection * frameHeight);

			prevWDown = wDown;
			prevSDown = sDown;
			prevADown = aDown;
			prevDDown = dDown;

			prevUpDown = upDown;
			prevDownDown = downDown;
			prevLeftDown = leftDown;
			prevRightDown = rightDown;

			secTensRect.x = colonRect.x + txtColonW;
			secTensTexture = numbers[secTens].tex;
			secTensRect.w = numbers[secTens].w;
			secTensRect.h = numbers[secTens].h;

			secUnitsRect.x = secTensRect.x + numbers[secTens].w;
			secUnitsTexture = numbers[secUnits].tex;
			secUnitsRect.w = numbers[secUnits].w;
			secUnitsRect.h = numbers[secUnits].h;

			minUnitsRect.x = colonRect.x - numbers[minUnits].w;
			minUnitsTexture = numbers[minUnits].tex;
			minUnitsRect.w = numbers[minUnits].w;
			minUnitsRect.h = numbers[minUnits].h;
			
			minTensRect.x = minUnitsRect.x - numbers[minTens].w;
			minTensTexture = numbers[minTens].tex;
			minTensRect.w = numbers[minTens].w;
			minTensRect.h = numbers[minTens].h;

			int P1PointsTens = floor(p1points / 10);
			int P1PointsUnits = p1points - 10 * P1PointsTens;
			int P2PointsTens = floor(p2points / 10);
			int P2PointsUnits = p2points - 10 * P2PointsTens;

			P1PointsTensRect.x = txtP1Rect.x + txtP1Rect.w + 10;
			P1PointsTensTexture = numbers[P1PointsTens].tex;
			P1PointsTensRect.w = numbers[P1PointsTens].w;
			P1PointsTensRect.h = numbers[P1PointsTens].h;

			P1PointsUnitsRect.x = P1PointsTensRect.x + numbers[P1PointsUnits].w;
			P1PointsUnitsTexture = numbers[P1PointsUnits].tex;
			P1PointsUnitsRect.w = numbers[P1PointsUnits].w;
			P1PointsUnitsRect.h = numbers[P1PointsUnits].h;

			P2PointsTensRect.x = txtP2Rect.x + txtP2Rect.w + 10;
			P2PointsTensTexture = numbers[P2PointsTens].tex;
			P2PointsTensRect.w = numbers[P2PointsTens].w;
			P2PointsTensRect.h = numbers[P2PointsTens].h;

			P2PointsUnitsRect.x = P2PointsTensRect.x + numbers[P2PointsUnits].w;
			P2PointsUnitsTexture = numbers[P2PointsUnits].tex;
			P2PointsUnitsRect.w = numbers[P2PointsUnits].w;
			P2PointsUnitsRect.h = numbers[P2PointsUnits].h;

			//Mouse Clicked

			//Buttons

			// DRAW
			SDL_RenderClear(m_renderer);
			//Background
			SDL_RenderCopy(m_renderer, bgGameTexture, nullptr, &bgRect);

			//Separation Bar
			SDL_RenderCopy(m_renderer, separationBarTexture, nullptr, &sepBar0);
			SDL_RenderCopy(m_renderer, separationBarTexture, nullptr, &sepBar1);
			SDL_RenderCopy(m_renderer, separationBarTexture, nullptr, &sepBar2);
			SDL_RenderCopy(m_renderer, separationBarTexture, nullptr, &sepBar3);
			SDL_RenderCopy(m_renderer, separationBarTexture, nullptr, &sepBar4);

			for (int i = 0; i < goldVector.size(); ++i)
			{
				SDL_RenderCopy(m_renderer, goldTexture, nullptr, &goldVector[i]);
			}

			SDL_RenderCopy(m_renderer, playerTexture, &player1Rect, &player1Position);
			SDL_RenderCopy(m_renderer, playerTexture, &player2Rect, &player2Position);

			SDL_RenderCopy(m_renderer, txtColon, nullptr, &colonRect);
			SDL_RenderCopy(m_renderer, secUnitsTexture, nullptr, &secUnitsRect);
			SDL_RenderCopy(m_renderer, secTensTexture, nullptr, &secTensRect);
			SDL_RenderCopy(m_renderer, minUnitsTexture, nullptr, &minUnitsRect);
			SDL_RenderCopy(m_renderer, minTensTexture, nullptr, &minTensRect);

			SDL_RenderCopy(m_renderer, txtP1Texture, nullptr, &txtP1Rect);
			SDL_RenderCopy(m_renderer, txtP2Texture, nullptr, &txtP2Rect);

			SDL_RenderCopy(m_renderer, P1PointsTensTexture, nullptr, &P1PointsTensRect);
			SDL_RenderCopy(m_renderer, P1PointsUnitsTexture, nullptr, &P1PointsUnitsRect);

			SDL_RenderCopy(m_renderer, P2PointsTensTexture, nullptr, &P2PointsTensRect);
			SDL_RenderCopy(m_renderer, P2PointsUnitsTexture, nullptr, &P2PointsUnitsRect);

			SDL_RenderPresent(m_renderer);
		}
		else if (scene == Scene::ENDGAME) 
		{
			// UPDATE
			deltaTime = (clock() - previousTime) / float(CLOCKS_PER_SEC);
			previousTime = clock();
			cursorRect.x = mouseX;
			cursorRect.y = mouseY;

			if (playTimer > 0.0f) {
				playTimer -= deltaTime;
			}
			else if (playTimer < 0.0f) {
				playTimer = 0.0f;
			}

			//ESC
			if (escClicked)
			{
				scene = Scene::MENU;
				escClicked = false;
			}

			//Mouse Click
			if (mouseClicked && mouseOnPlay && playTimer == 0)
			{
				playTimer = ButtonCd;
				scene = Scene::MENU;
				mouseClicked = false;
			}

			//Buttons
			if ((mouseX < (btnMenuRect.x + btnMenuRect.w) && mouseX > btnMenuRect.x) &&
				(mouseY < (btnMenuRect.y + btnMenuRect.h) && mouseY >(btnMenuRect.y)))
			{
				//Mouse on play button		
				mouseOnPlay = true;
			}
			else
			{
				mouseOnPlay = false;
			}

			if (mouseOnPlay) {
				if (mouseClicked) {
					btnMenuTexture = btnClickTexture;
				}
				else {
					btnMenuTexture = btnHoverTexture;
				}
			}
			else {
				btnMenuTexture = btnNormalTexture;
			}

			// DRAW
			SDL_RenderClear(m_renderer);
			//Background
			SDL_RenderCopy(m_renderer, bgMenuTexture, nullptr, &bgRect);

			//Buttons
			SDL_RenderCopy(m_renderer, btnMenuTexture, nullptr, &btnMenuRect);

			//Text
			SDL_RenderCopy(m_renderer, txtMenuTexture, nullptr, &txtMenuRect);
			SDL_RenderCopy(m_renderer, txtGameOverTexture, nullptr, &txtGameOverRect);
			SDL_RenderCopy(m_renderer, WinnerTexture, nullptr, &WinnerRect);

			//Cursor
			SDL_RenderCopy(m_renderer, cursorTexture, nullptr, &cursorRect);
			SDL_RenderPresent(m_renderer);
		}
	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(bgMenuTexture);
	SDL_DestroyTexture(bgGameTexture);
	SDL_DestroyTexture(btnClickTexture);
	SDL_DestroyTexture(btnHoverTexture);
	SDL_DestroyTexture(btnNormalTexture);
	SDL_DestroyTexture(cursorTexture);
	SDL_DestroyTexture(txtPlayTexture);
	SDL_DestroyTexture(txtQuitTexture);
	SDL_DestroyTexture(txtMuteTexture);
	SDL_DestroyTexture(txtSoundOnTexture);
	SDL_DestroyTexture(txtStartOffTexture);
	SDL_DestroyTexture(txtStartOnTexture);
	SDL_DestroyTexture(charSelBgNormalTexture);
	SDL_DestroyTexture(charSelBgSelectedTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(playerTextureLocked);
	SDL_DestroyTexture(txtSelectCharacterTexture);
	SDL_DestroyTexture(txtPlayerOneTexture);
	SDL_DestroyTexture(txtPlayerTwoTexture);
	SDL_DestroyTexture(separationBarTexture);
	SDL_DestroyTexture(txtColon);
	SDL_DestroyTexture(zero);
	SDL_DestroyTexture(one);
	SDL_DestroyTexture(two);
	SDL_DestroyTexture(three);
	SDL_DestroyTexture(four);
	SDL_DestroyTexture(five);
	SDL_DestroyTexture(six);
	SDL_DestroyTexture(seven);
	SDL_DestroyTexture(eight);
	SDL_DestroyTexture(nine);
	SDL_DestroyTexture(txtP1Texture);
	SDL_DestroyTexture(txtP2Texture);
	SDL_DestroyTexture(txtGameOverTexture);
	SDL_DestroyTexture(P1WinsTexture);
	SDL_DestroyTexture(P2WinsTexture);
	SDL_DestroyTexture(DrawTexture);
	SDL_DestroyTexture(txtMenuTexture);

	IMG_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}