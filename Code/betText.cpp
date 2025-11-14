#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class BetText {
    public:
        double x = 700.0;
        double y = 405.0;
        int width = 96;
        int height = 96;
        int fontSize = 96;
        int money = 0;
        TTF_Font* font = NULL;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Color textColor = {255, 255, 255, 0};

    bool load(SDL_Renderer* renderer) {
        font = TTF_OpenFont("./Assets/Fonts/BLACKJACK.ttf", fontSize);
        if (!font) {
            std::cerr << "Error loading Score Font.\n";
            return false;
        }
        std::string text = ("Bet:  $" + std::to_string(money));
        width = text.length() * 20;
        textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        if (!textSurface) {
            std::cerr << "Error: Text Surface Could not be loaded\n";
            return false;
        }
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        if (!textTexture) {
            std::cerr << "Error: Text Texture Could Not Be Loaded\n";
            return false;
        }
        return true;
    }

    void render(SDL_Renderer* renderer, int changedMoney) {
        if (changedMoney != money) {
            money = changedMoney;
            std::string text = ("Bet:  $" + std::to_string(money));
            width = text.length() * 20;
            textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
            if (!textSurface) {
                std::cerr << "Score Surface could not be created. SDL Error: " << SDL_GetError() << '\n';
            }
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);
            if (!textTexture) {
                std::cerr << "Score Texture could not be created. SDL Error: " << SDL_GetError() << '\n';
            }
        }
        SDL_Rect textRect;
        textRect.x = x;
        textRect.y = y;
        textRect.w = width;
        textRect.h = height;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }
};
