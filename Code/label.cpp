#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class LabelText {
    public:
        double x = 850.0;
        double y = 20.0;
        std::string labelText = "BLACKJACK";
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
};
