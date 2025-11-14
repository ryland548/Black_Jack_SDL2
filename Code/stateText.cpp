#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class StateText {
    public:
        double x = 50.0;
        double y = 380.0;
        std::string labelText = "Make a Bet: ";
        int width = labelText.length() * 20;
        int height = 70;
        int fontSize = 96;
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
        textSurface = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
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

    void render(SDL_Renderer* renderer) {
        SDL_Rect textRect;
        textRect.x = x;
        textRect.y = y;
        textRect.w = width;
        textRect.h = height;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    void changeText(std::string &text, SDL_Renderer* renderer) {
        labelText = text;
        width = labelText.length() * 20;
        textSurface = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
        if (!textSurface) {
            std::cerr << "Text Surface could not be created. SDL Error: " << SDL_GetError() << '\n';
        }
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        if (!textTexture) {
            std::cerr << "Text Texture could not be created. SDL Error: " << SDL_GetError() << '\n';
        }
    }
};
