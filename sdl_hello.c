#include <stdio.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdbool.h>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *w = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
	SDL_Renderer *r = SDL_CreateRenderer(w,-1,SDL_RENDERER_SOFTWARE);
	IMG_Init(0);
	SDL_Surface * surf = IMG_Load("test.jpg");
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
	rect.w = 0.2*(680);
	rect.h = hw_ratio * rect.w;
	//rect.w = 50;
	//rect.h = 100;
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
				default:
					break;
			}
		}
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r,255,255,255,255);
		SDL_RenderDrawLine(r,0,0,640,480);
		SDL_RenderDrawLine(r,640,0,0,480);
		SDL_RenderFillRect(r,&rect);
		SDL_RenderCopy(r,tex,NULL,&rect);
		rect.x+=4;
		rect.y+=3;
		if(rect.y > 480)
		{
			rect.y = 0;
		}
		if(rect.x > 640)
		{
			rect.x = 0;
		}
		SDL_SetRenderDrawColor(r,0,0,0,255);
		SDL_RenderPresent(r);
		SDL_Delay(1000/60);
	}
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return EXIT_SUCCESS;
}