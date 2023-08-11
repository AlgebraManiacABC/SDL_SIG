#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *w = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
	SDL_Renderer *r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT, 2, 2048))
    {
        fprintf(stderr,"Mix_OpenAudio failure: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

	SDL_RenderClear(r);
	SDL_RenderPresent(r);
	Mix_Music * ambience = Mix_LoadMUS("night_ambience.mp3");
	if(!ambience)
    {
        fprintf(stderr,"Music creation failure: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

	Mix_PlayMusic(ambience,-1);
	SDL_bool close = SDL_FALSE;
	while(!close)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					close = SDL_TRUE;
					break;
				default:
					break;
			}
		}
	}

	Mix_CloseAudio();
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return 0;
}