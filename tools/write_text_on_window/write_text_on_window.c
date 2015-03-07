#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

void write_text_on_window(SDL_Surface* window, int x, int y, int font_size, int red, int green,
                            int blue, char* text) {
    SDL_Surface *text_surface = NULL;

    SDL_Rect surface_position; // Will contain x and y coordinates to blit surfaces on the window.
    surface_position.x = x;
    surface_position.y = y;

    TTF_Font *font = NULL;
    SDL_Color color = {red, green, blue};

    TTF_Init();

    font = TTF_OpenFont("font/ubuntu-b.ttf", font_size); // Loading the font.

    text_surface = TTF_RenderUTF8_Blended(font, text, color);

    // We blit the text to the window.
    SDL_BlitSurface(text_surface, NULL, window, &surface_position);
    SDL_Flip(window);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_FreeSurface(text_surface);
}
