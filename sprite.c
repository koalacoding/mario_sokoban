#include "sprite.h"

Sprite* sprite_create(const char* image) {
    Sprite* sprite = malloc(sizeof(Sprite));
    memset(sprite, 0, sizeof(Sprite));

    sprite->image[DIRECTION_UP] = IMG_Load(image);
    if (sprite->image[DIRECTION_UP] == NULL) {
        return NULL;
    }
    sprite->allocated_image[DIRECTION_UP] = true;

    sprite->image[DIRECTION_DOWN] = sprite->image[DIRECTION_UP];
    sprite->image[DIRECTION_LEFT] = sprite->image[DIRECTION_UP];
    sprite->image[DIRECTION_RIGHT] = sprite->image[DIRECTION_UP];

    return sprite;
}

Sprite* sprite_create_faced(const char* image_up, const char* image_down,
                            const char* image_left, const char* image_right) {
    Sprite* sprite = NULL;

    SDL_Surface* surface_up = NULL;
    SDL_Surface* surface_down = NULL;
    SDL_Surface* surface_left = NULL;
    SDL_Surface* surface_right = NULL;

    surface_up = IMG_Load(image_up);
    if (surface_up == NULL) {
        // see http://www.libsdl.org/projects/docs/SDL_image/SDL_image_7.html
        // about return code
        fprintf(stderr, "failed to load %s\n", IMG_GetError());
        goto end;
    }

    surface_down = IMG_Load(image_down);
    if (surface_down == NULL) {
        fprintf(stderr, "failed to load %s\n", IMG_GetError());
        goto end;
    }

    surface_left = IMG_Load(image_left);
    if (surface_left == NULL) {
        fprintf(stderr, "failed to load %s\n", IMG_GetError());
        goto end;
    }

    surface_right = IMG_Load(image_right);
    if (surface_right == NULL) {
        fprintf(stderr, "failed to load %s\n", IMG_GetError());
        goto end;
    }

    sprite = malloc(sizeof(Sprite));
    // this should never happened, program should exit if it does so a lot of
    // developpers don't bother to check as it adds a lot of code
    if (sprite == NULL) {
        fprintf(stderr, "Allocation failed %s:%d\n", __FILE__, __LINE__);
        goto end;
    }
    memset(sprite, 0, sizeof(Sprite));

    // commit changes now that everything 'risky' is done
    sprite->allocated_image[DIRECTION_UP] = true;
    sprite->image[DIRECTION_UP] = surface_up;

    sprite->allocated_image[DIRECTION_DOWN] = true;
    sprite->image[DIRECTION_DOWN] = surface_down;
    sprite->allocated_image[DIRECTION_LEFT] = true;
    sprite->image[DIRECTION_LEFT] = surface_left;
    sprite->allocated_image[DIRECTION_RIGHT] = true;
    sprite->image[DIRECTION_RIGHT] = surface_right;

end:
    // cleanup everything if anything went wrong
    if (sprite == NULL) {
        if (surface_up) {
            SDL_FreeSurface(surface_up);
        }
        if (surface_down) {
            SDL_FreeSurface(surface_down);
        }
        if (surface_left) {
            SDL_FreeSurface(surface_left);
        }
        if (surface_right) {
            SDL_FreeSurface(surface_right);
        }
    }
    return sprite;
}

void sprite_destroy(Sprite* sprite) {
    if (sprite->allocated_image[DIRECTION_UP] && sprite->image[DIRECTION_UP]) {
        SDL_FreeSurface(sprite->image[DIRECTION_UP]);
        // ensure any use will crash immediately
        sprite->image[DIRECTION_UP] = NULL;
    }
    if (sprite->allocated_image[DIRECTION_DOWN] && sprite->image[DIRECTION_DOWN]) {
        SDL_FreeSurface(sprite->image[DIRECTION_DOWN]);
        sprite->image[DIRECTION_DOWN] = NULL;
    }
    if (sprite->allocated_image[DIRECTION_LEFT] && sprite->image[DIRECTION_LEFT]) {
        SDL_FreeSurface(sprite->image[DIRECTION_LEFT]);
        sprite->image[DIRECTION_LEFT] = NULL;
    }
    if (sprite->allocated_image[DIRECTION_RIGHT] && sprite->image[DIRECTION_RIGHT]) {
        SDL_FreeSurface(sprite->image[DIRECTION_RIGHT]);
        sprite->image[DIRECTION_RIGHT] = NULL;
    }
    free(sprite);
}


// unreferenced
// destroy every sprite item of a sprite list with sprite_destroy(), but
// doesn't free the list
// TODO: Sprite list could be a struct ?
void sprite_list_reset(Sprite** sprites, unsigned int sprite_count)
{
    unsigned int i = 0;
    for (i = 0; i < sprite_count; i++) {
        if (sprites[i] != NULL)
            continue;
        sprite_destroy(sprites[i]);
        sprites[i] = NULL;
    }
}
