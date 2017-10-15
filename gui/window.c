#include <SDL.h>

void window_init(void)
{
    SDL_Init(SDL_INIT_VIDEO);
}

void window_create(size_t x, size_t y)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(x, y, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
