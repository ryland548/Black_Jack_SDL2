#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Card {
    public:
        int x = 50;
        int y = 600;
        int gap = 80;
        int width = 691 / 5;
        int height = 1056 / 5;
        int cardValue = 0;
        std::string suit = "";
        std::string cardId = "";
        int dir = 0;
        bool visible = true;
        bool loaded = false;
        SDL_Surface* spriteSurface = nullptr;
        SDL_Texture* spriteTexture = nullptr;
        SDL_Surface* backSpriteSurface = nullptr;
        SDL_Texture* backSpriteTexture = nullptr;
        std::string cardPath = "";
        SDL_Rect spriteRect;

    void load(SDL_Renderer* renderer, int randomNum) {
        loaded = true;
        cardPath = "./Assets/Sprites/Cards/" + cardId + suit + ".png";
        spriteSurface = IMG_Load(cardPath.c_str());
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);
        if (randomNum == 1) {
            backSpriteSurface = IMG_Load("./Assets/Sprites/Cards/red_back.png");
        } else if (randomNum == 2) {
            backSpriteSurface = IMG_Load("./Assets/Sprites/Cards/yellow_back.png");
        } else if (randomNum == 3) {
            backSpriteSurface = IMG_Load("./Assets/Sprites/Cards/purple_back.png");
        } else if (randomNum == 4) {
            backSpriteSurface = IMG_Load("./Assets/Sprites/Cards/green_back.png");
        } else if (randomNum == 5) {
            backSpriteSurface = IMG_Load("./Assets/Sprites/Cards/blue_back.png");
        }
        backSpriteTexture = SDL_CreateTextureFromSurface(renderer, backSpriteSurface);
        SDL_FreeSurface(backSpriteSurface);
    }

    void render(SDL_Renderer* renderer) {
        spriteRect.x = x;
        spriteRect.y = y;
        spriteRect.w = width;
        spriteRect.h = height;
        SDL_Point spriteCenterPoint = {spriteRect.w / 2, spriteRect.h / 2};
        if (visible) {
            SDL_RenderCopyEx(renderer, spriteTexture, NULL, &spriteRect, dir, &spriteCenterPoint, SDL_FLIP_NONE);
        } else {
            SDL_RenderCopyEx(renderer, backSpriteTexture, NULL, &spriteRect, dir, &spriteCenterPoint, SDL_FLIP_NONE);
        }
    }
};
