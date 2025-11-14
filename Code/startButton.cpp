#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class StartButton {
    public:
        int x = 340;
        int y = 305;
        int width = 150;
        int height = 60;
        std::string buttonLabel = "START";
        TTF_Font* font = nullptr;
        SDL_Surface* textSurface = nullptr;
        SDL_Texture* textTexture = nullptr;
        SDL_Color textColor = {255, 255, 255, 0};
        int fontSize = 40;
        int textW = 0;
        int textH = 0;

    bool load(SDL_Renderer* renderer) {
        font = TTF_OpenFont("./Assets/Fonts/BLACKJACK.ttf", fontSize);
        if (!font) {
            std::cout << "Font could not be loaded. SDL Error: " << TTF_GetError() << std::endl;
            return false;
        }
        textSurface = TTF_RenderText_Solid(font, buttonLabel.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        return true;
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect rect = {
             x,
             y,
             width,
             height
        };
        SDL_SetRenderDrawColor(renderer, 245, 206, 66, 0);
        SDL_RenderFillRect(renderer, &rect);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {
            x + (width - textW) / 2,
            y + (height - textH) / 2,
            textW,
            textH
        };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    bool isClicked(int mouseX, int mouseY) {
        return mouseX >= x && mouseX <= x + width &&
               mouseY >= y && mouseY <= y + height;
    }
};
