//#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>

#define LEFT_PART_OF_SPRITE_TO_TOE_DIST (25.0/50.0)
#define FPS 60
#define RPS 1	//rotations per second
#define DPS (360*(RPS/(float)FPS))	//degrees per second
#define FPQ (FPS/4.0)
#define Q0 0.0
#define Q1 FPQ
#define Q2 (FPQ*2.0)
#define Q3 (FPQ*3.0)
#define SPEED 5

#define MOV_UP 0b0001
#define MOV_DOWN 0b0010
#define MOV_LEFT 0b0100
#define MOV_RIGHT 0b1000

#define DIR_LEFT false
#define DIR_RIGHT true
typedef bool direction;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *w = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
	SDL_Renderer *r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	IMG_Init(0);
	SDL_Surface * surf = IMG_Load("spin_1.png");
	if(!surf)
	{
		fprintf(stderr,"IMG_Load fail: %s\n",IMG_GetError());
		return EXIT_FAILURE;
	}
	SDL_Texture * tex = SDL_CreateTextureFromSurface(r,surf);
	SDL_FreeSurface(surf);
	bool close = false;
	SDL_Rect rect = {0,0,0,0};
	SDL_QueryTexture(tex,NULL,NULL,&rect.w,&rect.h);
	float hw_ratio = rect.h/(float)rect.w;
	float def_w = 0.2*(680);
	rect.w = def_w;
	rect.h = hw_ratio * rect.w;
	float real_x = rect.w/2;
	float real_y = 0;
	Uint8 movement = 0x00;
	bool spin = false;
	int spin_countdown = 0;
	direction dir = DIR_LEFT;
	float theta = 0;
	Uint8 keyup = 0x00;
	while(!close)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					close = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode)
					{
						case SDL_SCANCODE_SPACE:
							if(!spin)
							{
								spin = true;
								spin_countdown = FPS*2;
							}
							break;
						case SDL_SCANCODE_W:
							movement |= MOV_UP;
							break;
						case SDL_SCANCODE_A:
							movement |= MOV_LEFT;
							break;
						case SDL_SCANCODE_S:
							movement |= MOV_DOWN;
							break;
						case SDL_SCANCODE_D:
							movement |= MOV_RIGHT;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.scancode)
					{
						case SDL_SCANCODE_W:
							movement &= ~MOV_UP;
							break;
						case SDL_SCANCODE_A:
							movement &= ~MOV_LEFT;
							break;
						case SDL_SCANCODE_S:
							movement &= ~MOV_DOWN;
							break;
						case SDL_SCANCODE_D:
							movement &= ~MOV_RIGHT;
							break;
						default:
							break;
					}
				default:
					break;
			}
		}
		SDL_RenderClear(r);

		if(movement & MOV_UP)
			real_y -= SPEED;
		if(movement & MOV_DOWN)
			real_y += SPEED;
		if(movement & MOV_LEFT)
		{
			if(dir != DIR_LEFT && !spin)
			{
				dir = DIR_LEFT;
				theta += 180;
			}
			real_x -= SPEED;
		}
		if(movement & MOV_RIGHT)
		{
			if(dir != DIR_RIGHT && !spin)
			{
				dir = DIR_RIGHT;
				theta += 180;
			}
			real_x += SPEED;
		}
		
		float real_w = round(fabs(cos((M_PI*theta)/180.0))*(def_w));
		rect.w = real_w;
		rect.x = real_x - (real_w / 2);
		rect.y = real_y;
		
		if(spin)
		{
			spin_countdown--;
			theta += DPS;
			if(spin_countdown == 0)
				spin = false;
		}
		if(theta >= 360)
			theta -= 360;

		switch(((int)theta)/90)
		{
			case 0:
			//	Mario faces left (no flip), width shrinking, x increasing
				SDL_RenderCopyEx(r,tex,NULL,&rect,0,NULL,SDL_FLIP_NONE);
				break;
			case 1:
			//	Mario faces right (flip), width gaining, x decreasing
				SDL_RenderCopyEx(r,tex,NULL,&rect,0,NULL,SDL_FLIP_HORIZONTAL);
				break;
			case 2:
			//	Mario faces right (flip), width shrinking, x increasing
				SDL_RenderCopyEx(r,tex,NULL,&rect,0,NULL,SDL_FLIP_HORIZONTAL);
				break;
			case 3:
			//	Mario faces left (no flip), width gaining, x decreasing
				SDL_RenderCopyEx(r,tex,NULL,&rect,0,NULL,SDL_FLIP_NONE);
				break;
			default:
				fprintf(stderr,"Unexpected case! (theta = %.1f)\n",theta);
				break;
		}

		SDL_RenderPresent(r);
		SDL_Delay(1000/FPS);
	}
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return EXIT_SUCCESS;
}