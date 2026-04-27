#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>


#define WINDOWS_WIDTH 1600
#define WINDOWS_HEIGHT 900
#define RECT_WIDTH 300
#define RECT_HEIGHT 180
#define SIZE 30

uint64_t last_heal;
uint64_t heal_second;
uint64_t random_second;

typedef struct mouse {
	int x;
	int y;
} Mouse;
Mouse mouse; // Mouse Position

typedef struct player {
	int x1;
	int y1;
	int x2;
	int y2;
	SDL_Texture *texture_player1;
	SDL_Texture *texture_player2;
} Player; 
Player player; // Player Position

typedef struct bullet {
	int x1;
	int y1;
	int x2;
	int y2;
	SDL_Texture *texture_bullet1;
	SDL_Texture *texture_bullet2;
} Bullet; 
Bullet bullet; // Attack Position

typedef struct {
	int p1;
	int p2;
	int d1;
	int d2;
} HP; 
HP hp; // Player HP
// p = Player, d = Damage

typedef struct {
	int x;
	int y;
	SDL_Texture *texture_healer;
} Healer; 
Healer healer; // Healer

int win; // 1 = Player 1, 2 = Player 2, 0 = Draw
int pla = 0;
const int ms = 5;
const int healhp = 10;
int target;
bool run = true; // Program running
bool start, play, rules, quit;
bool attack1,attack2;
bool heal;
SDL_Rect rectp1, rectp2, rectb1, rectb2;
SDL_Rect rectheal;
SDL_Color colorWin;

void status(int s){
	switch (s){
	case 1: // Start
		start = true;
		rules = false;
		play = false;
		quit = false;
		break;
	case 2: // Play
		start = false;
		rules = false;
		play = true;
		quit = false;
		player.x1 = 260;
		player.y1 = 340;
		player.x2 = 1340 - SIZE;
		player.y2 = 340;
		attack1 = false;
		attack2 = false;
		heal = false;
		hp.p1 = 100;
		hp.p2 = 100;
		break;
	case 3: // Rules
		start = false;
		rules = true;
		play = false;
		quit = false;
		attack1 = false;
		attack2 = false;
		heal = false;
		player.x1 = 260;		
		player.y1 = 360;
		player.x2 = 1340 - SIZE;
		player.y2 = 360;
		break;
	case 4: // Quit
		start = false;
		rules = false;
		play = false;
		quit = true;
		break;
	case 5:
		run = false;
	}
	random_second = rand() % 3000 + 2000;
	last_heal = SDL_GetTicks();
}

void set_background(SDL_Renderer *renderer, SDL_Texture *background){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = WINDOWS_WIDTH;
    rect.h = WINDOWS_HEIGHT;
    SDL_RenderCopy(renderer, background, NULL, &rect);
}

void renderText(const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {x, y, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void renderDynamicText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, const char *format, ...) {
    char text[256];
    va_list args;
    va_start(args, format);
    vsnprintf(text, sizeof(text), format, args);
    va_end(args);
    renderText(text, renderer, font, color, x, y);
}

void button_box(SDL_Renderer *renderer) {
	for (int i = 0; i < 3; i++){
		if (mouse.x > WINDOWS_WIDTH/2 - RECT_WIDTH/2 && 
			mouse.x < WINDOWS_WIDTH/2 + RECT_WIDTH/2 && 
			mouse.y > (i * 250 + 120) &&
			mouse.y < (i * 250 + 120 + RECT_HEIGHT))
				SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		else
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_Rect rect;
		rect.x = WINDOWS_WIDTH/2 - RECT_WIDTH/2;
		rect.y = i * 250 + 120;
		rect.w = RECT_WIDTH;
		rect.h = RECT_HEIGHT;
		SDL_RenderFillRect(renderer, &rect);
	}
}

void button_text(SDL_Renderer *renderer,TTF_Font *font,SDL_Color color){
	char text[32];
	for (int i = 0; i < 3; i++){
		switch (i){
		case 0:
			strcpy(text,"PLAY");
			break;
		case 1:
			strcpy(text,"RULE");
			break;
		case 2:
			strcpy(text,"QUIT");
			break;
		}
		renderText(text, renderer, font, color, WINDOWS_WIDTH/2 - 100, i * 250 + 170);
	}
}

void button_click(SDL_Event event){
	if (event.type = SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && 
		mouse.x > (WINDOWS_WIDTH/2 - RECT_WIDTH/2) && mouse.x < (WINDOWS_WIDTH/2 + RECT_WIDTH/2)) {
		if (mouse.y > 120 && mouse.y < 120 + RECT_HEIGHT)
			status(2);
		else if (mouse.y > 370 && mouse.y < 370 + RECT_HEIGHT)
			status(3);
		else if (mouse.y > 650 && mouse.y < 650 + RECT_HEIGHT)
			status(4);
		else
			status(1);
	}
}

void rules_text(SDL_Renderer *renderer,TTF_Font *font,SDL_Color color1,SDL_Color color2,SDL_Color color3){
	SDL_Color color;
	char text[64];
	int xpos;
	int ypos;

	for (int i = 0; i < 6; i++){
		if (i < 3)
			xpos = 200;
		else 
			xpos = (WINDOWS_WIDTH/2);
		
		if (i == 0 || i == 3)
			ypos = 100;
		else if (i == 1 || i == 4)
			ypos = 180;
		else if (i == 2 || i == 5)
			ypos = 260;

		if (i == 0)
			color = color2;
		else if (i == 3)
			color = color3;
		else
			color = color1;

		switch (i){
			case 0:
				strcpy(text,"PLAYER 1:");
				break;
			case 1:
				strcpy(text,"Move: W, S, A, D");
				break;
			case 2:
				strcpy(text,"Attack: E");
				break;
			case 3:
				strcpy(text,"PLAYER 2:");
				break;
			case 4:
				strcpy(text,"Move: UP, DOWN, LEFT, RIGHT");
				break;
			case 5:
				strcpy(text,"Attack: ENTER");
				break;
		}
		renderText(text,renderer,font,color,xpos,ypos);
	}
}

void play_text(SDL_Renderer *renderer,TTF_Font *font,SDL_Color color1,SDL_Color color2){
	SDL_Color color;
	char text[64];
	int xpos1 = 250;
	int ypos1 = 100;
	int xpos2 = WINDOWS_WIDTH/2 + 100;
	int ypos2 = 220;

	renderDynamicText(renderer, font, color1, xpos1, ypos1, "P1 X: %d, Y: %d", player.x1, player.y1);
	renderDynamicText(renderer, font, color2, xpos2, ypos1, "P2 X: %d, Y: %d", player.x2, player.y2);
	renderDynamicText(renderer, font, color1, xpos1, ypos2, "P1 HP: %d", hp.p1);
	renderDynamicText(renderer, font, color2, xpos2, ypos2, "P2 HP: %d", hp.p2);
}

void hp_change(SDL_Renderer *renderer,TTF_Font *font,SDL_Color green,SDL_Color red,SDL_Color blue){
	int x1 = WINDOWS_WIDTH/2 + 400;
	int x2 = WINDOWS_WIDTH/2 - 250;
	int y = 220;
	int sec = 3000;

	if ((hp.d1 != 0 || hp.d2 != 0) && (SDL_GetTicks() - heal_second < sec)){
		if (hp.d1 > 0) renderDynamicText(renderer, font, green, x2, y, "+%d", hp.d1);
		else if (hp.d1 < 0) renderDynamicText(renderer, font, blue, x2, y, "%d", hp.d1);
		if (hp.d2 > 0) renderDynamicText(renderer, font, green, x1, y, "+%d", hp.d2);
		else if (hp.d2 < 0) renderDynamicText(renderer, font, red, x1, y, "%d", hp.d2);
	} else {
		heal_second = SDL_GetTicks();
		hp.d1 = 0;
		hp.d2 = 0;
	}
}

void quit_text(SDL_Renderer *renderer,TTF_Font *font,SDL_Color color1,SDL_Color color2){
	SDL_Color color;
	char text[64];
	int xpos;
	int ypos = 500;
	int position = 250;

	renderText("COMFIRM ?", renderer, font, color1, WINDOWS_WIDTH/2 - 200, 300);
	for (int i = 0; i < 2; i++){
		switch (i){
		case 0:
			xpos = WINDOWS_WIDTH / 2 - position/2*3;
			strcpy(text,"YES");
			break;
		
		case 1:
			xpos = WINDOWS_WIDTH / 2 + position;
			strcpy(text,"NO");
			break;
		}

		if (mouse.x > xpos && mouse.x < xpos + position && mouse.y > ypos && mouse.y < ypos + 80)
			color = color2;
		else
			color = color1;
		renderText(text,renderer,font,color,xpos,ypos);
	}
}

void quit_click(SDL_Renderer *renderer,SDL_Event event){
	int xpos;
	int ypos = 500;
	int position = 250;

	for (int i = 0; i < 2; i++){
		switch (i){
		case 0:
			xpos = WINDOWS_WIDTH / 2 - position/2*3;
			break;
		
		case 1:
			xpos = WINDOWS_WIDTH / 2 + position;
			break;
		}

		if (event.type = SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && 
			mouse.x > xpos && mouse.x < xpos + position && mouse.y > ypos && mouse.y < ypos + 80)
			if (i == 0)
				status(5);
			else if (i == 1)
				status(1);
	}
}

// Set outside x and y position
// X is the space in the between of two box, Y is the position of two boxes
void draw_boxes(SDL_Renderer *renderer, int x, int y, int length){
	// White box
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
	SDL_Rect rectw1 = {(WINDOWS_WIDTH / 2) - length - (x/2),y,length,length};
	SDL_RenderFillRect(renderer, &rectw1);
	SDL_Rect rectw2 = {(WINDOWS_WIDTH / 2) + (x/2),y,length,length};
	SDL_RenderFillRect(renderer, &rectw2);

	// Black box
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
	SDL_Rect rectw3 = {(WINDOWS_WIDTH / 2) - length - (x/2) + 10,y + 10,length - 20,length - 20};
	SDL_RenderFillRect(renderer, &rectw3);
	SDL_Rect rectw4 = {(WINDOWS_WIDTH / 2) + (x/2) + 10,y + 10,length - 20,length - 20};
	SDL_RenderFillRect(renderer, &rectw4);
}

void draw_player(SDL_Renderer *renderer,Player player){
	SDL_Rect rect1;
	rect1.x = player.x1;
	rect1.y = player.y1;
	rect1.w = SIZE;
	rect1.h = SIZE;
	SDL_RenderCopy(renderer, player.texture_player1, NULL, &rect1);

	SDL_Rect rect2;
	rect2.x = player.x2;
	rect2.y = player.y2;
	rect2.w = SIZE;
	rect2.h = SIZE;
	SDL_RenderCopy(renderer, player.texture_player2, NULL, &rect2);
}

void draw_bullet(SDL_Renderer *renderer,Bullet bullet){
	SDL_Rect rect1;
	rect1.x = bullet.x1;
	rect1.y = bullet.y1;
	rect1.w = SIZE;
	rect1.h = (SIZE / 2);
	SDL_RenderCopy(renderer, bullet.texture_bullet1, NULL, &rect1);

	SDL_Rect rect2;
	rect2.x = bullet.x2;
	rect2.y = bullet.y2;
	rect2.w = SIZE;
	rect2.h = (SIZE / 2);
	SDL_RenderCopy(renderer, bullet.texture_bullet2, NULL, &rect2);
}

void draw_hpbox(SDL_Renderer *renderer, int x, int y, int hp){
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect recthp1 = {x,y,300,30};
	SDL_RenderFillRect(renderer, &recthp1);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_Rect recthp2 = {x,y,hp * 3,30};
	SDL_RenderFillRect(renderer, &recthp2);
}

void dark_box(SDL_Renderer *renderer,SDL_Rect rect,int xx,int yy,int ww,int hh){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	rect.x = xx;
	rect.y = yy;
	rect.w = ww;
	rect.h = hh;
	SDL_RenderFillRect(renderer, &rect);
}

void draw_healer(SDL_Renderer *renderer,Healer healer){
	SDL_Rect rect;
	rect.x = healer.x;
	rect.y = healer.y;
	rect.w = SIZE;
	rect.h = SIZE;
	SDL_RenderCopy(renderer, healer.texture_healer, NULL, &rect);
}

void control_player(const Uint8 *state, int up, int down, int left1, int right1, int left2, int right2){
	// Player 1
	if (state[SDL_SCANCODE_W] && player.y1 > up)
		player.y1 -= ms;
	else if (state[SDL_SCANCODE_S] && player.y1 < down - SIZE)
		player.y1 += ms;
	if (state[SDL_SCANCODE_A] && player.x1 > left1)
		player.x1 -= ms;
	else if (state[SDL_SCANCODE_D] && player.x1 < right1 - SIZE)
		player.x1 += ms;

	// Player 2
	if (state[SDL_SCANCODE_UP] && player.y2 > up)
		player.y2 -= ms;
	else if (state[SDL_SCANCODE_DOWN] && player.y2 < down - SIZE)
		player.y2 += ms;
	if (state[SDL_SCANCODE_LEFT] && player.x2 > left2)
		player.x2 -= ms;
	else if (state[SDL_SCANCODE_RIGHT] && player.x2 < right2 - SIZE)
		player.x2 += ms;
}

void control_bullet(const Uint8 *state,int left,int right){
	// Player 1
	if (attack1){
		bullet.x1 += ms*5;
	 	if (bullet.x1 > right || (bullet.x1 > player.x2 && (player.y2 - bullet.y1) < (SIZE/2) && (bullet.y1 - player.y2) < SIZE))
			attack1 = false;
	} else {
		bullet.x1 = player.x1;
		bullet.y1 = player.y1 + 5;
		if (state[SDL_SCANCODE_E])
			attack1 = true;
		else {
			bullet.x1 = WINDOWS_WIDTH;
			bullet.y1 = WINDOWS_HEIGHT;
		}
	}
	
	// Player 2
	if (attack2){
		bullet.x2 -= ms*5;
		if (bullet.x2 < left || (bullet.x2 < player.x1 && (player.y1 - bullet.y2) < (SIZE/2) && (bullet.y2 - player.y1) < SIZE))
			attack2 = false;
	} else {
		bullet.x2 = player.x2;
		bullet.y2 = player.y2 + 5;
		if (state[SDL_SCANCODE_RETURN])
			attack2 = true;
		else {
			bullet.x2 = WINDOWS_WIDTH;
			bullet.y2 = WINDOWS_HEIGHT;
		}
	}
}

void control_healer(int y1,int y2,int left,int right){
	if (heal){
		switch (target){
		case 0:
			healer.x -= ms;
			break;
		
		case 1:
			healer.x += ms;
			break;
		}
		if (healer.x < left ||
			healer.x > right ||
			(healer.x < player.x1 + SIZE && healer.x > player.x1 - SIZE*2 && healer.y < player.y1 + SIZE && healer.y > player.y1 - SIZE) ||
			(healer.x > player.x2 - SIZE && healer.x < player.x2 + SIZE*2 && healer.y < player.y2 + SIZE && healer.y > player.y2 - SIZE))
				heal = false;
	} else {
		if ((SDL_GetTicks() - last_heal) >= random_second){
			healer.x = (WINDOWS_WIDTH - SIZE) / 2;
			healer.y = rand() % (y2 - y1 - SIZE) + y1;
			last_heal = SDL_GetTicks();
			target = rand() % 2;
			random_second = rand() % 3000 + 2000;
			heal = true;
		} else {
			healer.x = WINDOWS_WIDTH / 2;
			healer.y = WINDOWS_HEIGHT;
		}
	}
}

// Damage → 5 <= x <= 10
void damage_calculate(){
	if (bullet.x2 < player.x1 && (player.y1 - bullet.y2) < (SIZE/2) && (bullet.y2 - player.y1) < SIZE){
		hp.d1 = -(rand() % 5 + 5);
		hp.p1 += hp.d1;
	} if (bullet.x1 > player.x2 && (player.y2 - bullet.y1) < (SIZE/2) && (bullet.y1 - player.y2) < SIZE){
		hp.d2 = -(rand() % 5 + 5);
		hp.p2 += hp.d2;
	} if (healer.x < player.x1 + SIZE && healer.x > player.x1 - SIZE*2 && healer.y < player.y1 + SIZE && healer.y > player.y1 - SIZE){
		hp.d1 = healhp;
		hp.p1 += hp.d1;
	} if (healer.x > player.x2 - SIZE && healer.x < player.x2 + SIZE*2 && healer.y < player.y2 + SIZE && healer.y > player.y2 - SIZE){
		hp.d2 = healhp;
		hp.p2 += hp.d2;
	}

	if (hp.p1 > 100) hp.p1 = 100;
	if (hp.p2 > 100) hp.p2 = 100;
}

void sound_effect(const Uint8 *state,Mix_Chunk *shoot,Mix_Chunk *hurt,Mix_Chunk *heal){
	if (state[SDL_SCANCODE_E] || state[SDL_SCANCODE_RETURN]){
		Mix_PlayChannel(-1, shoot,0);
	}
	
	if ((bullet.x2 < player.x1 && (player.y1 - bullet.y2) < (SIZE/2) && (bullet.y2 - player.y1) < SIZE) || (bullet.x1 > player.x2 && (player.y2 - bullet.y1) < (SIZE/2) && (bullet.y1 - player.y2) < SIZE)){
		Mix_HaltChannel(-1);
		Mix_PlayChannel(-1, hurt,0);
	}

	if ((healer.x < player.x1 + SIZE && healer.x > player.x1 - SIZE*2 && healer.y < player.y1 + SIZE && healer.y > player.y1 - SIZE) ||
		(healer.x > player.x2 - SIZE && healer.x < player.x2 + SIZE*2 && healer.y < player.y2 + SIZE && healer.y > player.y2 - SIZE)){
		Mix_HaltChannel(-1);
		Mix_PlayChannel(-1, heal,0);
	}
}

void end(SDL_Renderer *renderer, TTF_Font *font1, TTF_Font *font2, SDL_Color color, SDL_Color colora, const Uint8 *state){
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_Rect recte1 = {180,100,WINDOWS_WIDTH - 360,WINDOWS_HEIGHT - 200};
	SDL_RenderFillRect(renderer, &recte1);
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	SDL_Rect recte2 = {200,120,WINDOWS_WIDTH - 400,WINDOWS_HEIGHT - 240};
	SDL_RenderFillRect(renderer, &recte2);
	renderText("AGAIN ?", renderer, font1, color, 640, 380);
	renderText("OK",renderer, font2, colora, 680, 540);
	Mix_HaltChannel(-1);
}

void winner(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color){
	if (win == 1) {
		renderText("Player 1 Win!", renderer, font, color, 550, 210);
	} else if (win == 2) {
		renderText("Player 2 Win!", renderer, font, color, 550, 210);
	}
}

int main(int argc, char *args[]){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_Window *window = SDL_CreateWindow( "Games", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWS_WIDTH, WINDOWS_HEIGHT, 0);
	SDL_Surface* icon = NULL;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	TTF_Font *fontT = TTF_OpenFont("assets/arial.ttf", 30);
	TTF_Font *fontS = TTF_OpenFont("assets/arial.ttf", 50);
	TTF_Font *fontM = TTF_OpenFont("assets/arial.ttf", 80);
	TTF_Font *fontL = TTF_OpenFont("assets/arial.ttf", 120);
	SDL_Color cWhite = {255, 255, 255}, cGrey = {150,150,150}, cBlack = {0, 0, 0}, cRed = {255, 0, 0}, cGreen = {0, 255, 0}, cBlue = {0, 0, 255};
	SDL_Event event;
	
	SDL_Surface *img_background = IMG_Load("assets/bg.png");
	SDL_Surface *img_player1 = IMG_Load("assets/player1.png");
	SDL_Surface *img_bullet1 = IMG_Load("assets/bullet1.png");
	SDL_Surface *img_bullet2 = IMG_Load("assets/bullet2.png");
	SDL_Surface *img_player2 = IMG_Load("assets/player2.png");
	SDL_Surface *img_heal = IMG_Load("assets/heal.png");

	SDL_Texture *texture_background = SDL_CreateTextureFromSurface(renderer, img_background);
	SDL_Texture *texture_player1 = SDL_CreateTextureFromSurface(renderer, img_player1);
	SDL_Texture *texture_player2 = SDL_CreateTextureFromSurface(renderer, img_player2);
	SDL_Texture *texture_bullet1 = SDL_CreateTextureFromSurface(renderer, img_bullet1);
	SDL_Texture *texture_bullet2 = SDL_CreateTextureFromSurface(renderer, img_bullet2);
	SDL_Texture *texture_heal = SDL_CreateTextureFromSurface(renderer, img_heal);

	SDL_FreeSurface(img_background);
	SDL_FreeSurface(img_player1);
	SDL_FreeSurface(img_player2);
	SDL_FreeSurface(img_bullet1);
	SDL_FreeSurface(img_bullet2);
	SDL_FreeSurface(img_heal);

	icon = SDL_LoadBMP("assets/icon.bmp");
	SDL_SetWindowIcon(window, icon);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Chunk *sound_bullet = Mix_LoadWAV("assets/bullet.mp3");
	Mix_Chunk *sound_hit = Mix_LoadWAV("assets/hit.mp3");
	Mix_Chunk *sound_blink = Mix_LoadWAV("assets/blink.mp3");

	player.texture_player1 = texture_player1;
	player.texture_player2 = texture_player2;
	bullet.texture_bullet1 = texture_bullet1;
	bullet.texture_bullet2 = texture_bullet2;
	healer.texture_healer = texture_heal;

	status(1);
		
	while (run){
		// Background
		set_background(renderer,texture_background);
		renderText("Press TAB to Start Menu.", renderer, fontT, cWhite, 20, 20);
		renderText("Press ESC to Exit the Game.", renderer, fontT, cWhite, 20, 60);

		if (SDL_PollEvent(&event) > 0 && (event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) )
			run = false;
		
		if (state[SDL_SCANCODE_TAB])
			status(1);

		// Mouse position
		if (event.motion.x != 0 && event.motion.x < 1600 && event.motion.y != 0 && event.motion.y < 1000){
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
		}

		renderDynamicText(renderer, fontT, cWhite, 20, WINDOWS_HEIGHT - 50, "Mouse X: %d, Y: %d", mouse.x, mouse.y);

		if (start){
			// Start UI Button
			button_box(renderer);
			button_text(renderer,fontM,cBlack);
			button_click(event);

		} else if (rules){
			if (mouse.x > 1500 && mouse.x < 1580 && mouse.y > 30 && mouse.y < 130){
				renderText("X", renderer, fontL, cGrey, WINDOWS_WIDTH - 100, 20);
				if (event.type = SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
					status(1);
			} else
				renderText("X", renderer, fontL, cWhite, WINDOWS_WIDTH - 100, 20);
			
			rules_text(renderer,fontS,cWhite,cRed,cBlue);

			draw_boxes(renderer,200,350,450);
			draw_bullet(renderer,bullet);
			draw_player(renderer,player);
			draw_healer(renderer,healer);
			
			control_bullet(state,220,1360);
			control_player(state,360,790,260,690,910,1340);
			control_healer(360,790,260,1360);
			sound_effect(state,sound_bullet,sound_hit,sound_blink);
			
		} else if (play){
			play_text(renderer,fontS,cRed,cBlue);
			hp_change(renderer,fontS,cGreen,cRed,cBlue);

			dark_box(renderer,rectp1,player.x1,player.y1,SIZE,SIZE);
			dark_box(renderer,rectp2,player.x2,player.y2,SIZE,SIZE);
			dark_box(renderer,rectb1,bullet.x1,bullet.y1,SIZE,SIZE/2);
			dark_box(renderer,rectb2,bullet.x2,bullet.y2,SIZE,SIZE/2);

			draw_boxes(renderer,300,330,400);
			draw_hpbox(renderer,250,170,hp.p1);
			draw_hpbox(renderer,WINDOWS_WIDTH / 2 + 100,170,hp.p2);
			draw_bullet(renderer,bullet);
			draw_player(renderer,player);
			draw_healer(renderer,healer);
			
			if (hp.p1 > 0 && hp.p2 > 0){	
				control_player(state,340,720,260,640,960,1340);
				control_healer(340,720,230,1370);
				control_bullet(state,230,1370);
				sound_effect(state,sound_bullet,sound_hit,sound_blink);
				damage_calculate();
				
			} else {
				if (hp.p1 > 0) win = 1;
				else if (hp.p2 > 0) win = 2;
				else win = 0;

				if (win == 1) colorWin = cRed;
				else if (win == 2) colorWin = cBlue;

				if (mouse.x > 680 && mouse.x < 860 && mouse.y > 540 && mouse.y < 650)
					if (event.type = SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
						status(2);
					else
						end(renderer, fontM,fontL,cWhite,cGrey,state);
				else
					end(renderer, fontM,fontL,cWhite,cWhite,state);
				
				winner(renderer, fontM, colorWin);

				hp.p1 = (hp.p1 < 0) ? 0 : hp.p1;
				hp.p2 = (hp.p2 < 0) ? 0 : hp.p2;
			}

		} else if (quit){
			quit_text(renderer,fontM,cWhite,cGrey);
			quit_click(renderer,event);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(5);
	}

	SDL_DestroyTexture(texture_background);
	SDL_DestroyTexture(texture_player1);
	SDL_DestroyTexture(texture_player2);
	SDL_DestroyTexture(texture_bullet1);
	SDL_DestroyTexture(texture_bullet2);
	SDL_DestroyTexture(texture_heal);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(fontS);
	TTF_CloseFont(fontM);
	TTF_CloseFont(fontL);
	Mix_FreeChunk(sound_bullet);
	Mix_FreeChunk(sound_hit);
	Mix_FreeChunk(sound_blink);
	Mix_CloseAudio();

	TTF_Quit();
	SDL_Quit();
	return 0;
}